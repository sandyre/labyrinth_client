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
        PlayerConnection(const std::string& name, uint32_t uuid)
        : Name(name),
          Uuid(uuid)
        { }

        const std::string Name;
        const uint32_t Uuid;
    };

    class GameSearchScene
        : public cocos2d::Scene
    {
    public:
        GameSearchScene();

        virtual bool init() override;

    private:
        virtual void update(float delta) override;

        void MessageHandler(const MessageBufferPtr& message)
        { std::lock_guard<std::mutex> l(_mutex); _messages.push_back(message); }

        void MasterMessageHandler(const MessageBufferPtr& message);

    private:
        boost::signals2::connection _socketConnection;
        TcpSocketPtr                _socket;

        boost::signals2::scoped_connection  _masterSocketConnection;
        UdpSocketPtr                        _masterSocket;

        std::mutex                  _mutex;
        MessageStorage              _messages;

        ui::GameSearch *            _ui;

        std::vector<PlayerConnection>   _playerConnections;
    };

}

#endif /* GameSearchScene_hpp */
