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
m_eState(State::WAITING)
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
    cocos2d::Vec2 spritePos = LOG_TO_PHYS_COORD(pos,
                                                m_pSprite->getContentSize());
    auto move = cocos2d::MoveTo::create(0.5, spritePos);
    
    m_pSprite->runAction(move);
}

void
Monster::AnimationDeath()
{
    auto fade = cocos2d::FadeOut::create(0.5);
    auto rotate = cocos2d::RotateBy::create(1.0, 1080);
    auto seq = cocos2d::Sequence::create(rotate, fade, nullptr);
    this->m_pSprite->runAction(seq);
}
