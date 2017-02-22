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
    enum Status
    {
        REQUESTING_LOBBY,
        CONNECTING_TO_GS,
        WAITING_ACCEPT,
        WAITING_OTHERS,
        READY_TO_START,
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
    
    GameScene * m_pGameScene; // building
    
    CREATE_FUNC(PreGameScene);
};

#endif /* pregamescene_hpp */
