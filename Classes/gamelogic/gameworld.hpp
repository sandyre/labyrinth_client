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
#include "../gsnet_generated.h"
#include "../netsystem.hpp"
#include "../ui/ui_gamescene.hpp"

#include <cocos2d.h>

#include <vector>
#include <queue>


class GameObject;
struct GameSessionDescriptor;

class GameWorld
{
public:
    GameWorld(GameSessionDescriptor&);
    
    virtual void update(float);
    
    cocos2d::Layer * GetView()
    { return _view; }

    void SetHUD(UIGameScene * ui);
    
    const std::shared_ptr<Hero> GetLocalPlayer()
    { return _localPlayer; }

protected:
    void ReceiveInputNetEvents();
    void SendOutgoingNetEvents();

protected:
    GameMap::Configuration                  _mapConf;
    std::shared_ptr<NetChannel>             _channel;
    std::queue<std::vector<uint8_t>>        _outEvents;
    std::deque<std::shared_ptr<GameObject>> _objects;
        
        // just points to an Unit in vector
    std::shared_ptr<Hero>                   _localPlayer;
    
    cocos2d::Layer *                        _view;
    UIGameScene *                           _ui;
    
    friend GameMap;
    friend Unit;
    friend Hero;
    friend Warrior;
    friend Mage;
    friend Monster;
};

#endif /* gameworld_hpp */
