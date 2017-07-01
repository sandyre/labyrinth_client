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
#include <mutex>

class NetChannel : public Poco::Task
{
public:
    NetChannel(const Poco::Net::SocketAddress&);
    ~NetChannel();
    
    virtual void            runTask() override;
    
    bool                    Available() const;
    void                    PushPacket(const std::vector<uint8_t>& data);
    std::vector<uint8_t>    PopPacket();

private:
    mutable std::mutex                  _mutex;
    std::array<uint8_t, 512>            _buffer;
    std::deque<std::vector<uint8_t>>    _packetsDeque;
    
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
        _taskManager.cancelAll();
    }
    
    void                                CreateChannel(const std::string& name,
                                                      const Poco::Net::SocketAddress& adr);
    void                                RemoveChannel(const std::string& name);
    
    std::shared_ptr<NetChannel>         GetChannel(const std::string& name);

protected:
    Poco::TaskManager                                   _taskManager;
    std::map<std::string, std::shared_ptr<NetChannel>>  _channels;
};

#endif /* netsystem_hpp */
