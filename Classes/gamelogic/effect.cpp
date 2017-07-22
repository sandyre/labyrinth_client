//
//  effect.cpp
//  labyrinth
//
//  Created by Aleksandr Borzikh on 11.05.17.
//
//

#include "effect.hpp"

#include "units/unit.hpp"

#include <cocos2d.h>


Effect::Effect(Unit * targetUnit, float duration)
    : _targetUnit(targetUnit),
      _duration(duration)
{
    assert(targetUnit);
}


void
Effect::update(float delta)
{
    if(_state == Effect::State::ACTIVE)
    {
        _duration -= delta;
        if(_duration <= 0.0f)
            _state = Effect::State::OVER;
    }
}


WarriorDash::WarriorDash(Unit * targetUnit, float duration, float bonusMovespeed)
: Effect(targetUnit, duration),
  _bonusMovespeed(bonusMovespeed)
{ }


void
WarriorDash::start()
{ _targetUnit->m_nMoveSpeed += _bonusMovespeed; }


void
WarriorDash::stop()
{ _targetUnit->m_nMoveSpeed -= _bonusMovespeed; }


WarriorArmorUp::WarriorArmorUp(Unit * targetUnit, float duration, int16_t bonusArmor)
: Effect(targetUnit, duration),
  _bonusArmor(bonusArmor)
{ }


void
WarriorArmorUp::start()
{ _targetUnit->m_nArmor += _bonusArmor; }


void
WarriorArmorUp::stop()
{ _targetUnit->m_nArmor -= _bonusArmor; }


RogueInvisibility::RogueInvisibility(Unit * targetUnit, float duration)
: Effect(targetUnit, duration)
{ }


void
RogueInvisibility::start()
{
    _targetUnit->_objAttributes &= ~(GameObject::Attributes::VISIBLE);
    _targetUnit->m_nUnitAttributes &= ~(Unit::Attributes::DUELABLE);
    auto fadeOut = cocos2d::FadeOut::create(0.5);
    _targetUnit->GetSprite->runAction(fadeOut);
}


void
RogueInvisibility::stop()
{
    _targetUnit->_objAttributes |= (GameObject::Attributes::VISIBLE);
    _targetUnit->m_nUnitAttributes |= (Unit::Attributes::DUELABLE);
    auto fadeIn = cocos2d::FadeIn::create(0.5);
    _targetUnit->GetSprite()->runAction(fadeIn);
}


MageFreeze::MageFreeze(Unit * targetUnit, float duration)
: Effect(targetUnit, duration)
{ }


void
MageFreeze::start()
{ _targetUnit->m_nUnitAttributes &= ~(Unit::Attributes::INPUT); }


void
MageFreeze::stop()
{ _targetUnit->m_nUnitAttributes |= Unit::Attributes::INPUT; }


DuelInvulnerability::DuelInvulnerability(Unit * targetUnit, float duration)
: Effect(targetUnit, duration)
{ }


void
DuelInvulnerability::start()
{ _targetUnit->m_nUnitAttributes &= ~(Unit::Attributes::DUELABLE); }


void
DuelInvulnerability::stop()
{ _targetUnit->m_nUnitAttributes |= Unit::Attributes::DUELABLE; }


RespawnInvulnerability::RespawnInvulnerability(Unit * targetUnit, float duration)
: Effect(targetUnit, duration)
{ }


void
RespawnInvulnerability::start()
{
    _targetUnit->_objAttributes &= ~(GameObject::Attributes::PASSABLE);
    _targetUnit->m_nUnitAttributes &= ~(Unit::Attributes::DUELABLE);
}


void
RespawnInvulnerability::stop()
{
    _targetUnit->_objAttributes |= GameObject::Attributes::PASSABLE;
    _targetUnit->m_nUnitAttributes |= Unit::Attributes::DUELABLE;
}
