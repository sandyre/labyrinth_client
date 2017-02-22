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

class NetSystem
{
public:
    static NetSystem& Instance()
    {
        static NetSystem ns;
        return ns;
    }
    
    Poco::Net::DatagramSocket&
    Socket()
    {
        return m_oDS;
    }
    
protected:
    Poco::Net::DatagramSocket m_oDS;
};

#endif /* netsystem_hpp */
