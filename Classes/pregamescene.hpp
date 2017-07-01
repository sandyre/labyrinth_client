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
#include "ui/ui_pregamescene.hpp"

#include <cocos2d.h>
#include <Poco/Net/SocketAddress.h>
#include <ui/CocosGUI.h>

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
    
protected:
    Status           m_eStatus;
    
    std::shared_ptr<NetChannel> _channel;
    
    std::vector<PlayerInfo> m_aLobbyPlayers;
    GameMap::Configuration m_stMapConfig;
    GameScene * m_pGameScene; // building
    
    UIPregameScene * m_pUI;
    
    CREATE_FUNC(PreGameScene);
};

#endif /* pregamescene_hpp */
