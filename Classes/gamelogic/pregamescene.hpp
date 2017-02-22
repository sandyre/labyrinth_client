//
//  pregamescene.hpp
//  labyrinth
//
//  Created by Aleksandr Borzikh on 04.02.17.
//
//

#ifndef pregamescene_hpp
#define pregamescene_hpp

#include <cocos2d.h>
#include <Poco/Net/DatagramSocket.h>
#include <Poco/Net/SocketAddress.h>

#include <gameworld.hpp>

class PreGameScene : public cocos2d::Layer
{
public:
    enum class State
    {
        CONNECTING_TO_MS,
        CONNECTING_TO_GS,
        WAITING_PLAYERS,
        WAITING_PLAYERS_READY,
        GENERATING_LEVEL,
        READY_TO_START,
        ENTERING_GAME
    };
public:
    static cocos2d::Scene * createScene();
    
    virtual bool init();
    void    update(float delta);
    
    CREATE_FUNC(PreGameScene);
    
protected:
    PreGameScene::State         m_eState;
    
    Poco::Net::SocketAddress    m_stGSAddr;
    cocos2d::Label *            m_pStatusLabel;
    GameWorld *                 m_pGameWorld; // PREGAMESCENE GENERATES GAMEWORLD!
};

#endif /* pregamescene_hpp */
