//
//  effect.cpp
//  labyrinth
//
//  Created by Aleksandr Borzikh on 11.05.17.
//
//

#include "effect.hpp"
#include "units/warrior.hpp"

#include <cocos2d.h>

Effect::Effect() :
m_nADuration(0.0f),
m_eEffState(Effect::State::ACTIVE)
{
    
}

Effect::~Effect()
{
    
}

Effect::State
Effect::GetState() const
{
    return m_eEffState;
}

void
Effect::SetTargetUnit(Unit * target)
{
    m_pTargetUnit = target;
}

WarriorDash::WarriorDash(float duration,
                         float bonus_movespeed) :
m_nBonusMovespeed(bonus_movespeed)
{
    m_nADuration = duration;
}

void
WarriorDash::start()
{
    m_pTargetUnit->m_nMoveSpeed -= m_nBonusMovespeed;
}

void
WarriorDash::update(float delta)
{
    if(m_eEffState == Effect::State::ACTIVE)
    {
        m_nADuration -= delta;
        if(m_nADuration < 0.0f)
        {
            m_eEffState = Effect::State::OVER;
        }
    }
}

void
WarriorDash::stop()
{
    m_pTargetUnit->m_nMoveSpeed += m_nBonusMovespeed;
}

void
WarriorArmorUp::start()
{
    m_pTargetUnit->m_nArmor += m_nBonusArmor;
}

WarriorArmorUp::WarriorArmorUp(float duration,
                               int16_t bonus_armor) :
m_nBonusArmor(bonus_armor)
{
    m_nADuration = duration;
}

void
WarriorArmorUp::update(float delta)
{
    if(m_eEffState == Effect::State::ACTIVE)
    {
        m_nADuration -= delta;
        if(m_nADuration < 0.0f)
        {
            m_eEffState = Effect::State::OVER;
        }
    }
}

void
WarriorArmorUp::stop()
{
    m_pTargetUnit->m_nArmor -= m_nBonusArmor;
}

RogueInvisibility::RogueInvisibility(float duration)
{
    m_nADuration = duration;
}

void
RogueInvisibility::start()
{
    m_pTargetUnit->m_nObjAttributes &= ~(GameObject::Attributes::VISIBLE);
    m_pTargetUnit->m_nUnitAttributes &= ~(Unit::Attributes::DUELABLE);
    auto fadeOut = cocos2d::FadeOut::create(0.5);
    m_pTargetUnit->runAction(fadeOut);
}

void
RogueInvisibility::update(float delta)
{
    if(m_eEffState == Effect::State::ACTIVE)
    {
        m_nADuration -= delta;
        if(m_nADuration < 0.0f)
        {
            m_eEffState = Effect::State::OVER;
        }
    }
}

void
RogueInvisibility::stop()
{
    m_pTargetUnit->m_nObjAttributes |= (GameObject::Attributes::VISIBLE);
    m_pTargetUnit->m_nUnitAttributes |= (Unit::Attributes::DUELABLE);
    auto fadeIn = cocos2d::FadeIn::create(0.5);
    m_pTargetUnit->runAction(fadeIn);
}

MageFreeze::MageFreeze(float duration)
{
    m_nADuration = duration;
}

void
MageFreeze::start()
{
    m_pTargetUnit->m_nUnitAttributes &= ~(Unit::Attributes::INPUT);
}

void
MageFreeze::update(float delta)
{
    if(m_eEffState == Effect::State::ACTIVE)
    {
        m_nADuration -= delta;
        if(m_nADuration < 0.0f)
        {
            m_eEffState = Effect::State::OVER;
        }
    }
}

void
MageFreeze::stop()
{
    m_pTargetUnit->m_nUnitAttributes |= Unit::Attributes::INPUT;
}

DuelInvulnerability::DuelInvulnerability(float duration)
{
    m_nADuration = duration;
}

void
DuelInvulnerability::start()
{
    m_pTargetUnit->m_nUnitAttributes &= ~(Unit::Attributes::DUELABLE);
}

void
DuelInvulnerability::update(float delta)
{
    if(m_eEffState == Effect::State::ACTIVE)
    {
        m_nADuration -= delta;
        if(m_nADuration < 0.0f)
        {
            m_eEffState = Effect::State::OVER;
        }
    }
}

void
DuelInvulnerability::stop()
{
    m_pTargetUnit->m_nUnitAttributes |= Unit::Attributes::DUELABLE;
}

RespawnInvulnerability::RespawnInvulnerability(float duration)
{
    m_nADuration = duration;
}

void
RespawnInvulnerability::start()
{
    m_pTargetUnit->m_nUnitAttributes &= ~(Unit::Attributes::DUELABLE);
    m_pTargetUnit->m_nObjAttributes &= ~(GameObject::Attributes::PASSABLE);
}

void
RespawnInvulnerability::update(float delta)
{
    if(m_eEffState == Effect::State::ACTIVE)
    {
        m_nADuration -= delta;
        if(m_nADuration < 0.0f)
        {
            m_eEffState = Effect::State::OVER;
        }
    }
}

void
RespawnInvulnerability::stop()
{
    m_pTargetUnit->m_nUnitAttributes |= Unit::Attributes::DUELABLE;
    m_pTargetUnit->m_nObjAttributes |= GameObject::Attributes::PASSABLE;
}
