//
//  pregamescene.hpp
//  labyrinth
//
//  Created by Aleksandr Borzikh on 07.02.17.
//
//

#ifndef pregamescene_hpp
#define pregamescene_hpp

#include "gamescene.hpp"
#include <cocos2d.h>
#include <Poco/Net/SocketAddress.h>

class PreGameScene : public cocos2d::Layer
{
public:
    struct PlayerConnectionInfo
    {
        uint32_t    nUID;
        std::string sNickname;
        Hero::Type  eHeroPicked;
    };
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
    cocos2d::Label * m_pLobbyInfo;
    cocos2d::Label * m_pStatusInfo;
    Poco::Net::SocketAddress m_stGSAddr;
    
    cocos2d::Sprite * m_pAirElem;
    cocos2d::Sprite * m_pFireElem;
    cocos2d::Sprite * m_pWaterElem;
    cocos2d::Sprite * m_pEarthElem;
    
    cocos2d::Sprite * m_pReadyButton;
    std::vector<PlayerConnectionInfo> m_aLobbyPlayers;
    GameMap::Configuration m_stMapConfig;
    GameScene * m_pGameScene; // building
    
    CREATE_FUNC(PreGameScene);
};

#endif /* pregamescene_hpp */
