//
//  netsystem.hpp
//  labyrinth
//
//  Created by Aleksandr Borzikh on 07.02.17.
//
//

#ifndef netsystem_hpp
#define netsystem_hpp

#include <Poco/Net/DatagramSocket.h>

#include <array>
#include <chrono>
using namespace std::chrono;

class NetSystem
{
public:
    enum ChannelState
    {
        DR_DONE,
        DR_FAILED,
        DR_TIMEOUT
    };
    class NetChannel
    {
    public:
        NetChannel();
        
        ChannelState    GetState() const;
        
        void            SetAddress(Poco::Net::SocketAddress);
        const Poco::Net::SocketAddress& GetAddress() const;
        int             DataAvailable() const;
        int             SendBytes(const void*, int);
        void            ReceiveBytes();
        
        const std::array<uint8_t, 512>& GetBuffer() const;
    protected:
        ChannelState                m_eCState;
        std::array<uint8_t, 512>    m_aDBuffer;
        Poco::Net::SocketAddress    m_oSockAddr;
        Poco::Net::DatagramSocket   m_oDSocket;
    };
public:
    static NetSystem&   Instance();
    NetChannel&         GetChannel(size_t);
protected:
    NetSystem();
    std::vector<NetChannel> m_aChannels;
};

#endif /* netsystem_hpp */
