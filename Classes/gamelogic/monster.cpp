//
//  monster.cpp
//  labyrinth
//
//  Created by Aleksandr Borzikh on 17.01.17.
//
//

#include "monster.hpp"

#include "cocos2d.h"

Monster::Monster() :
m_eState(State::WAITING),
m_nHealth(0)
{
    m_eObjType = GameObject::Type::MONSTER;
}

Monster *
Monster::create(const std::string& filename)
{
    Monster * pMonster = new Monster();
    
    if(pMonster->initWithFile(filename))
    {
        pMonster->autorelease();
        return pMonster;
    }
    
    CC_SAFE_DELETE(pMonster);
    return nullptr;
}

Monster::State
Monster::GetState() const
{
    return m_eState;
}

void
Monster::SetState(Monster::State state)
{
    m_eState = state;
}

uint16_t
Monster::GetHealth() const
{
    return m_nHealth;
}

void
Monster::SetHealth(uint16_t val)
{
    m_nHealth = val;
}

void
Monster::AnimationSpawn()
{
    auto fadeIn = cocos2d::FadeIn::create(1);
    this->runAction(fadeIn);
}

void
Monster::AnimationDeath()
{
    auto fadeOut = cocos2d::FadeOut::create(1);
    this->runAction(fadeOut);
}

void
Monster::AnimationMoveTo(cocos2d::Vec2 pos)
{
    auto moveTo = cocos2d::MoveTo::create(0.5, pos);
    this->runAction(moveTo);
}
