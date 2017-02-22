//
//  netsystem.hpp
//  labyrinth
//
//  Created by Aleksandr Borzikh on 05.02.17.
//
//

#ifndef netsystem_hpp
#define netsystem_hpp

#include <Poco/Net/SocketAddress.h>
#include <Poco/Net/DatagramSocket.h>

class NetSystem
{
public:
    static NetSystem& Instance()
    {
        static NetSystem sys;
        return sys;
    }
    
    Poco::Net::DatagramSocket& Socket()
    {
        return m_oSocket;
    }
private:
    NetSystem() {}
    ~NetSystem() {}
    
    NetSystem(NetSystem const&) = delete;
    NetSystem& operator=(NetSystem const&) = delete;
    
private:
    Poco::Net::DatagramSocket m_oSocket;
};

#endif /* netsystem_hpp */
