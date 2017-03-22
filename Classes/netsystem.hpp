//
//  netsystem.hpp
//  labyrinth
//
//  Created by Aleksandr Borzikh on 07.02.17.
//
//

#ifndef netsystem_hpp
#define netsystem_hpp

#include <chrono>
#include <Poco/Net/DatagramSocket.h>
using namespace std::chrono;

class NetSystem
{
public:
    static NetSystem& Instance()
    {
        static NetSystem ns;
        return ns;
    }
    
    void    SetAddress(const Poco::Net::SocketAddress& addr)
    {
        m_oCurrentAddr = addr;
    }
    
    const Poco::Net::SocketAddress& GetAddress() const
    {
        return m_oCurrentAddr;
    }
    
    int     DataAvailable() const
    {
        return m_oDS.available();
    }
    
    int     SendBytes(const void * buffer,
                     size_t lenght)
    {
        return m_oDS.sendTo(buffer, lenght, m_oCurrentAddr);
    }
    
    int     ReceiveBytes(void * buffer,
                         size_t buffer_capacity)
    {
        int  bytes_received = 0;
        auto cur_time = steady_clock::now();
        try
        {
            bytes_received = m_oDS.receiveBytes(buffer, buffer_capacity);
            m_oTimeSinceLastRcv = duration_cast<milliseconds>(cur_time - m_oLastRcv);
            m_oLastRcv = cur_time;
        }
        catch(std::exception& e) {}
        
        return bytes_received;
    }
    
    milliseconds GetTimeSinceLastReceive() const
    {
        return m_oTimeSinceLastRcv;
    }
    
protected:
    NetSystem()
    {
        m_oDS.setReceiveTimeout(Poco::Timespan(1, 0));
    }
    
    milliseconds               m_oTimeSinceLastRcv;
    steady_clock::time_point   m_oLastRcv;
    Poco::Net::SocketAddress  m_oCurrentAddr;
    Poco::Net::DatagramSocket m_oDS;
};

#endif /* netsystem_hpp */
