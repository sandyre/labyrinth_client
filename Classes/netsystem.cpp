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
: _address(addr)
{
    _socket.setReceiveTimeout(Poco::Timespan(0, 100000));
}


void
NetChannel::PushPacket(const std::vector<uint8_t> &data)
{
    _socket.sendTo(data.data(),
                   data.size(),
                   _address);
}


void
NetSystem::CreateChannel(const std::string& name, const Poco::Net::SocketAddress& adr)
{
    std::shared_ptr<NetChannel> channel = std::make_shared<NetChannel>(adr);
    _channels.insert(std::make_pair(name, channel));
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
