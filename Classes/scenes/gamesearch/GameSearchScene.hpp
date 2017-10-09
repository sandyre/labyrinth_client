//
//  GameSearchScene.hpp
//  labyrinth
//
//  Created by Alexandr Borzykh on 13/09/2017.
//
//

#ifndef GameSearchScene_hpp
#define GameSearchScene_hpp

#include "scenes/gamesearch/ui/ui_GameSearch.hpp"

#include "toolkit/TcpSocket.hpp"
#include "toolkit/UdpSocket.hpp"

#include <cocos2d.h>

#include <mutex>
#include <vector>


namespace labyrinth
{

    struct PlayerConnection
    {
        PlayerConnection() = default;
        PlayerConnection(const std::string& name, uint32_t uuid)
        : Name(name),
          Uuid(uuid)
        { }

        std::string Name;
        uint32_t Uuid;
    };

    class GameSearchScene
        : public cocos2d::Scene
    {
    public:
        virtual bool init() override;

    private:
        virtual void update(float delta) override;

        void MessageHandler(const MessageBufferPtr& message)
        { std::lock_guard<std::mutex> l(_mutex); _messages.push_back(message); }

        void MasterMessageHandler(const MessageBufferPtr& message);

        void ReadyClickedHandler();

    private:
        boost::signals2::connection _socketConnection;
        TcpSocketPtr                _socket;

        boost::signals2::scoped_connection  _masterSocketConnection;
        UdpSocketPtr                        _masterSocket;

        std::mutex                  _mutex;
        MessageStorage              _messages;

        ui::GameSearch *            _ui;

        PlayerConnection                _selfConnection;
        std::vector<PlayerConnection>   _playerConnections;

        boost::signals2::scoped_connection  _readyConnection;
    };

}

#endif /* GameSearchScene_hpp */
