//
//  pregamescene.hpp
//  labyrinth
//
//  Created by Aleksandr Borzikh on 07.02.17.
//
//

#ifndef pregamescene_hpp
#define pregamescene_hpp

#include "globals.h"
#include "gamescene.hpp"
#include "toolkit/TcpSocket.hpp"

#include "gamescene.hpp"

#include <cocos2d.h>
#include <Poco/Net/SocketAddress.h>
#include <ui/CocosGUI.h>

#include <mutex>


class UIPregameScene;

struct GameSessionDescriptor
{
    struct Player
    {
        uint32_t Uid;
        std::string Name;
        uint16_t Hero;
    };

    GameMap::Configuration  MapConf;
    Player                  LocalPlayer;
    std::vector<Player>     Players;
};

class PreGameScene : public cocos2d::Layer
{
public:
    enum Status
    {
        CONNECTING_TO_MS,
        REQUESTING_LOBBY,
        CONNECTING_TO_GS,
        WAITING_ACCEPT,
        WAITING_OTHERS,
        HERO_PICK_STAGE,
        GENERATING_LEVEL,
        WAITING_SERVER_START
    };

public:
    static cocos2d::Scene * createScene();
    
    virtual bool init();
    virtual void update(float);

private:
    void MessageHandler(const MessageBufferPtr& message)
    {
        std::lock_guard<std::mutex> l(_mutex);
        _messages.push_back(message);
    }
    
private:
    Status                      m_eStatus;

    std::mutex                  _mutex;
    MessageStorage              _messages;
    TcpSocketPtr                _socket;
    
    GameSessionDescriptor   _sessionDescriptor;
    GameMap::Configuration m_stMapConfig;
    GameScene * m_pGameScene; // building
    
    UIPregameScene * m_pUI;
    
    CREATE_FUNC(PreGameScene);
};

#endif /* pregamescene_hpp */
