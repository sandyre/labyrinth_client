//
//  netsystem.cpp
//  labyrinth
//
//  Created by Aleksandr Borzikh on 22.03.17.
//
//

#include "netsystem.hpp"

#include "gameconfig.hpp"

#include <fstream>

NetChannel::NetChannel(const Poco::Net::SocketAddress& addr)
 : Task("Channel")
{
    _socket.connect(addr);
}

NetChannel::~NetChannel()
{
    cancel();
    _socket.close();
}

bool
NetChannel::Available() const
{
    std::lock_guard<std::mutex> lock(_mutex);
    return !_packetsDeque.empty();
}

void
NetChannel::runTask()
{
    while(true)
    {
        size_t len = _socket.receiveBytes(_buffer.data(),
                                          _buffer.size());
        std::lock_guard<std::mutex> lock(_mutex);
        _packetsDeque.emplace_back(_buffer.data(),
                                   _buffer.data() + len);
    }
}

std::vector<uint8_t>
NetChannel::PopPacket()
{
    std::lock_guard<std::mutex> lock(_mutex);
    std::vector<uint8_t> pack = _packetsDeque.front();
    _packetsDeque.pop_front();
    return pack;
}

void
NetChannel::PushPacket(const std::vector<uint8_t> &data)
{
    _socket.sendBytes(data.data(),
                      data.size());
}

void
NetSystem::CreateChannel(const std::string& name, const Poco::Net::SocketAddress& adr)
{
    std::shared_ptr<NetChannel> channel = std::make_shared<NetChannel>(adr);
    _channels.insert(std::make_pair(name, channel));
    
    _taskManager.start(channel.get());
}

std::shared_ptr<NetChannel>
NetSystem::GetChannel(const std::string& name)
{
    return _channels[name];
}

void
NetSystem::RemoveChannel(const std::string& name)
{
    _channels.erase(name);
}
