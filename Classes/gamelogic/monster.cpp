//
//  monster.cpp
//  labyrinth
//
//  Created by Aleksandr Borzikh on 17.01.17.
//
//

#include "monster.hpp"

#include "cocos2d.h"

Monster::Monster()
{
    
}

uint16_t
Monster::GetUID() const
{
    return m_nUID;
}

void
Monster::SetUID(uint16_t uid)
{
    m_nUID = uid;
}

void
Monster::AnimationMoveTo(cocos2d::Vec2 pos)
{
    auto moveto = cocos2d::MoveTo::create(0.3, pos * CELL_SIZE);
    m_pSprite->runAction(moveto);
}
