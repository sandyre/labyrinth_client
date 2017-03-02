//
//  monster.hpp
//  labyrinth
//
//  Created by Aleksandr Borzikh on 17.01.17.
//
//

#ifndef monster_hpp
#define monster_hpp

#include "gameobject.hpp"
#include "globals.h"

class Monster : public GameObject
{
public:
    enum class State
    {
        WAITING,
        DUEL,
        DEAD
    };
public:
    static Monster* create(const std::string&);
    
    Monster::State  GetState() const;
    void            SetState(Monster::State);
    
    uint16_t    GetHealth() const;
    void        SetHealth(uint16_t);
    
    virtual void    AnimationSpawn();
    virtual void    AnimationDeath();
    virtual void    AnimationMoveTo(cocos2d::Vec2);
protected:
    Monster();
    
    Monster::State  m_eState;
    uint16_t        m_nHealth;
};

#endif /* monster_hpp */
