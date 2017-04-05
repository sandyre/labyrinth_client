//
//  netsystem.cpp
//  labyrinth
//
//  Created by Aleksandr Borzikh on 22.03.17.
//
//

#include "netsystem.hpp"

#include <fstream>

NetSystem::NetChannel::NetChannel()
{
    m_oDSocket.setReceiveTimeout(Poco::Timespan(1, 0));
}

NetSystem::ChannelState
NetSystem::NetChannel::GetState() const
{
    return m_eCState;
}

const std::array<uint8_t, 512>&
NetSystem::NetChannel::GetBuffer() const
{
    return m_aDBuffer;
}

const Poco::Net::SocketAddress&
NetSystem::NetChannel::GetAddress() const
{
    return m_oSockAddr;
}

void
NetSystem::NetChannel::SetAddress(Poco::Net::SocketAddress addr)
{
    m_oSockAddr = addr;
}

int
NetSystem::NetChannel::DataAvailable() const
{
    return m_oDSocket.available();
}

int
NetSystem::NetChannel::SendBytes(const void * buf,
                                 int size)
{
    return m_oDSocket.sendTo(buf, size, m_oSockAddr);
}

void
NetSystem::NetChannel::ReceiveBytes()
{
    try
    {
        m_oDSocket.receiveBytes(m_aDBuffer.data(), m_aDBuffer.size());
        m_eCState = ChannelState::DR_DONE;
    }
    catch(std::exception& e)
    {
        m_eCState = ChannelState::DR_TIMEOUT;
    }
}

NetSystem::NetSystem()
{
        // create 2 netchannels (1 = MS, 2 = GS)
    m_aChannels.push_back(NetChannel());
    
        // read MS addr and setup channel
    std::string ms_addr_t;
    std::ifstream net_file("res/net_info.txt");
    std::getline(net_file,ms_addr_t);
    net_file.close();
    
    Poco::Net::SocketAddress ms_addr(ms_addr_t);
    m_aChannels.front().SetAddress(ms_addr);
    
    m_aChannels.push_back(NetChannel());
}

NetSystem&
NetSystem::Instance()
{
    static NetSystem ns;
    return ns;
}

NetSystem::NetChannel&
NetSystem::GetChannel(size_t index)
{
    return m_aChannels[index];
}
