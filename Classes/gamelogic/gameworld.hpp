//
//  gameworld.hpp
//  labyrinth
//
//  Created by Aleksandr Borzikh on 15.04.17.
//
//

#ifndef gameworld_hpp
#define gameworld_hpp

#include "../globals.h"
#include "gamemap.hpp"
#include "gsnet_generated.h"
#include "units/units_inc.hpp"
#include "../ui/ui_gamescene.hpp"

#include <cocos2d.h>
#include <vector>
#include <queue>

class GameObject;

class GameWorld : public cocos2d::Layer
{
public:
    GameWorld();
    
    virtual void update(float);
    
    void    AddPlayer(PlayerInfo);
    void    CreateGameMap(const GameMap::Configuration&);
    
    void    SetHUD(UIGameScene * ui);
    
    Hero *  GetLocalPlayer();
protected:
    void    ReceiveInputNetEvents();
    void    SendOutgoingNetEvents();
protected:
    GameMap::Configuration  m_stMapConf;
    
        // contains outgoing events
    std::queue<std::vector<char>> m_aOutEvents;
    flatbuffers::FlatBufferBuilder builder;
    
        // basicly contains all objects on scene
    std::vector<GameObject*>    m_apoObjects;
        // just points to an Unit in vector
    Hero *   m_poLocalPlayer;
    
    UIGameScene * m_pUI;
    
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
