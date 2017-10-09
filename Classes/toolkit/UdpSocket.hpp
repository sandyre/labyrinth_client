//
//  UdpSocket.hpp
//  labyrinth
//
//  Created by Alexandr Borzykh on 17/09/2017.
//
//

#ifndef UdpSocket_hpp
#define UdpSocket_hpp

#include "Message.hpp"

#include <boost/signals2.hpp>

#include <Poco/NObserver.h>
#include <Poco/Net/DatagramSocket.h>
#include <Poco/Net/StreamSocket.h>
#include <Poco/Net/SocketNotification.h>
#include <Poco/Net/SocketReactor.h>

#include <mutex>

using Poco::AutoPtr;
using Poco::NObserver;
using Poco::Net::ReadableNotification;
using Poco::Net::WritableNotification;
using Poco::Net::ShutdownNotification;
using Poco::Net::ErrorNotification;


class UdpSocket
{
    using OnMessageSignal = boost::signals2::signal<void(const MessageBufferPtr&)>;

public:
    UdpSocket(const Poco::Net::SocketAddress& address)
    {
        _socket.connect(address);

        _reactor.addEventHandler(_socket, NObserver<UdpSocket, ReadableNotification>(*this, &UdpSocket::onSocketReadable));

        _worker.start(_reactor);
    }

    void SendMessage(const MessageBufferPtr& message)
    {
        std::lock_guard<std::mutex> l(_mutex);
        _outStorage.push_back(message);

        if (!_reactor.hasEventHandler(_socket, NObserver<UdpSocket, WritableNotification>(*this, &UdpSocket::onSocketWritable)))
            _reactor.addEventHandler(_socket, NObserver<UdpSocket, WritableNotification>(*this, &UdpSocket::onSocketWritable));
    }

    boost::signals2::scoped_connection OnMessageConnector(const OnMessageSignal::slot_type& slot)
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
            _reactor.removeEventHandler(_socket, NObserver<UdpSocket, WritableNotification>(*this, &UdpSocket::onSocketWritable));
    }

private:
    Poco::Net::DatagramSocket       _socket;

    std::mutex                      _mutex;
    MessageStorage                  _outStorage;
    MessageStorage                  _inStorage;

    Poco::Thread                    _worker;
    Poco::Net::SocketReactor        _reactor;

    OnMessageSignal                 _onMessage;
};
using UdpSocketPtr = std::shared_ptr<UdpSocket>;

#endif /* UdpSocket_hpp */
