//
//  TcpSocket.hpp
//  labyrinth
//
//  Created by Alexandr Borzykh on 09/09/2017.
//
//

#ifndef TcpSocket_hpp
#define TcpSocket_hpp

#include "Message.hpp"

#include <boost/signals2.hpp>

#include <Poco/NObserver.h>
#include <Poco/Net/StreamSocket.h>
#include <Poco/Net/SocketNotification.h>
#include <Poco/Net/SocketReactor.h>

using Poco::AutoPtr;
using Poco::NObserver;
using Poco::Net::ReadableNotification;
using Poco::Net::WritableNotification;
using Poco::Net::ShutdownNotification;
using Poco::Net::ErrorNotification;


class TcpSocket
{
    using OnMessageSignal = boost::signals2::signal<void(const MessageBufferPtr&)>;

public:
    TcpSocket(const Poco::Net::SocketAddress& address)
    {
        _socket.connect(address);
        _socket.setNoDelay(true);

        _reactor.addEventHandler(_socket, NObserver<TcpSocket, ReadableNotification>(*this, &TcpSocket::onSocketReadable));

        _worker.start(_reactor);
    }

    ~TcpSocket()
    { _reactor.stop(); }

    void SendMessage(const MessageBufferPtr& message)
    {
        std::lock_guard<std::mutex> l(_mutex);
        _outStorage.push_back(message);

        if (!_reactor.hasEventHandler(_socket, NObserver<TcpSocket, WritableNotification>(*this, &TcpSocket::onSocketWritable)))
            _reactor.addEventHandler(_socket, NObserver<TcpSocket, WritableNotification>(*this, &TcpSocket::onSocketWritable));
    }

    bool ConnectionAlive() const
    { return _alive; }

    boost::signals2::connection OnMessageConnector(const OnMessageSignal::slot_type& slot)
    {
        auto connection = _onMessage.connect(slot);

        std::lock_guard<std::mutex> l(_mutex);
        for (const auto& message : _inStorage)
            slot(message);
        _inStorage.clear();

        return connection;
    }

private:
    void onSocketReadable(const AutoPtr<ReadableNotification>& pNf)
    {
        if (_socket.available() == 0)
        {
            _alive = false;

            _reactor.removeEventHandler(_socket, NObserver<TcpSocket, ReadableNotification>(*this, &TcpSocket::onSocketReadable));
        }
        else if (_socket.available() >= 256)
        {
            auto message = std::make_shared<MessageBuffer>(256);

            _socket.receiveBytes(message->data(), message->size());

            if (!_onMessage.num_slots())
            {
                std::lock_guard<std::mutex> l(_mutex);
                _inStorage.push_back(message);
            }
            else
                _onMessage(message);
        }
    }

    void onSocketWritable(const AutoPtr<WritableNotification>& pNf)
    {
        std::lock_guard<std::mutex> l(_mutex);

        if (!_outStorage.empty())
        {
            const auto& message = _outStorage.front();
            message->resize(256);
            _socket.sendBytes(message->data(), message->size());
            _outStorage.pop_front();
        }
        else
            _reactor.removeEventHandler(_socket, NObserver<TcpSocket, WritableNotification>(*this, &TcpSocket::onSocketWritable));
    }

private:
    Poco::Net::StreamSocket     _socket;

    std::mutex                  _mutex;
    MessageStorage              _outStorage;
    MessageStorage              _inStorage;

    bool                        _alive;

    Poco::Thread                _worker;
    Poco::Net::SocketReactor    _reactor;

    OnMessageSignal             _onMessage;
};
using TcpSocketPtr = std::shared_ptr<TcpSocket>;

#endif /* TcpSocket_hpp */
