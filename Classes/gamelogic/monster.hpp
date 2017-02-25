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
    enum State
    {
        WAITING,
        DUEL,
        DEAD
    };
public:
    Monster();
    
    void        SetState(State state)
    {
        m_eState = state;
    }
    State       GetState() const
    {
        return m_eState;
    }
    
    uint16_t    GetHealth() const
    {
        return m_nHealth;
    }
    void        SetHealth(uint16_t val)
    {
        m_nHealth = val;
    }
    
    uint16_t    GetUID() const;
    void        SetUID(uint16_t);
    
    void                AnimationMoveTo(cocos2d::Vec2);
    void                AnimationDeath();
protected:
    State       m_eState;
    uint16_t    m_nUID;
    uint16_t    m_nHealth;
};

#endif /* monster_hpp */
