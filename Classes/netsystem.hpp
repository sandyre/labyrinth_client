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
#include <Poco/Runnable.h>
#include <Poco/TaskManager.h>
#include <Poco/Thread.h>

#include <array>
#include <chrono>
#include <deque>
#include <map>
#include <mutex>
#include <vector>

class NetChannel : public Poco::Runnable
{
public:
    NetChannel(const Poco::Net::SocketAddress&);
    ~NetChannel()
    {
        _running = false;
        _listeningThread.join();
        _socket.close();
    }
    
    bool                    Available() const;
    void                    PushPacket(const std::vector<uint8_t>& data);
    std::vector<uint8_t>    PopPacket();

private:
    void run();

private:
    mutable std::mutex                  _mutex;
    std::array<uint8_t, 512>            _buffer;
    std::deque<std::vector<uint8_t>>    _packetsDeque;

    bool                                _running;
    Poco::Thread                        _listeningThread;
    Poco::Net::SocketAddress            _address;
    Poco::Net::DatagramSocket           _socket;
};

class NetSystem
{
public:
    static NetSystem&   Instance()
    {
        static NetSystem ns;
        return ns;
    }
    ~NetSystem()
    {
    }
    
    void                                CreateChannel(const std::string& name,
                                                      const Poco::Net::SocketAddress& adr);
    void                                RemoveChannel(const std::string& name);
    
    std::shared_ptr<NetChannel>         GetChannel(const std::string& name);

protected:
    std::map<std::string, std::shared_ptr<NetChannel>>  _channels;
};

#endif /* netsystem_hpp */
