//
//  gameworld.hpp
//  labyrinth
//
//  Created by Aleksandr Borzikh on 15.04.17.
//
//

#ifndef gameworld_hpp
#define gameworld_hpp

#include "gamemap.hpp"
#include "units/units_inc.hpp"
#include "../globals.h"
#include "../netsystem.hpp"
#include "../gsnet_generated.h"
#include "../ui/ui_gamescene.hpp"

#include <cocos2d.h>

#include <vector>
#include <queue>

class GameObject;
struct GameSessionDescriptor;

class GameWorld : public cocos2d::Layer
{
public:
    GameWorld(GameSessionDescriptor&);
    
    virtual void update(float);
    
    void    SetHUD(UIGameScene * ui);
    
    Hero *  GetLocalPlayer();
protected:
    void    ReceiveInputNetEvents();
    void    SendOutgoingNetEvents();

protected:
    GameMap::Configuration  m_stMapConf;
    
        // contains outgoing events
    std::shared_ptr<NetChannel>     _channel; // gameserver channel
    std::queue<std::vector<uint8_t>> m_aOutEvents;
    flatbuffers::FlatBufferBuilder builder;
    
        // basicly contains all objects on scene
    std::vector<GameObject*>    m_apoObjects;
        // just points to an Unit in vector
    Hero *   m_poLocalPlayer;
    
    UIGameScene * m_pUI;
    
    cocos2d::Sprite *   m_pGameWorldSprite;
    
    friend GameMap;
    friend Unit;
    friend Hero;
    friend Rogue;
    friend Priest;
    friend Warrior;
    friend Mage;
    friend Monster;
};

#endif /* gameworld_hpp */
