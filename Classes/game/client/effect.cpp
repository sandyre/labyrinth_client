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


Effect::Effect(const std::shared_ptr<Unit>& targetUnit, float duration)
: _targetUnit(targetUnit),
  _state(Effect::State::ACTIVE),
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


WarriorDash::WarriorDash(const std::shared_ptr<Unit>& targetUnit, float duration, float bonusMovespeed)
: Effect(targetUnit, duration),
  _bonusMovespeed(bonusMovespeed)
{ }


void
WarriorDash::start()
{ _targetUnit->_moveSpeed += _bonusMovespeed; }


void
WarriorDash::stop()
{ _targetUnit->_moveSpeed -= _bonusMovespeed; }


WarriorArmorUp::WarriorArmorUp(const std::shared_ptr<Unit>& targetUnit, float duration, int16_t bonusArmor)
: Effect(targetUnit, duration),
  _bonusArmor(bonusArmor)
{ }


void
WarriorArmorUp::start()
{ _targetUnit->_armor += _bonusArmor; }


void
WarriorArmorUp::stop()
{ _targetUnit->_armor -= _bonusArmor; }


RogueInvisibility::RogueInvisibility(const std::shared_ptr<Unit>& targetUnit, float duration)
: Effect(targetUnit, duration)
{ }


void
RogueInvisibility::start()
{
    _targetUnit->_objAttributes &= ~(GameObject::Attributes::VISIBLE);
    _targetUnit->_unitAttributes &= ~(Unit::Attributes::DUELABLE);
    auto fadeOut = cocos2d::FadeOut::create(0.5);
    _targetUnit->GetSprite()->runAction(fadeOut);
}


void
RogueInvisibility::stop()
{
    _targetUnit->_objAttributes |= (GameObject::Attributes::VISIBLE);
    _targetUnit->_unitAttributes |= (Unit::Attributes::DUELABLE);
    auto fadeIn = cocos2d::FadeIn::create(0.5);
    _targetUnit->GetSprite()->runAction(fadeIn);
}


MageFreeze::MageFreeze(const std::shared_ptr<Unit>& targetUnit, float duration)
: Effect(targetUnit, duration)
{ }


void
MageFreeze::start()
{ _targetUnit->_unitAttributes &= ~(Unit::Attributes::INPUT); }


void
MageFreeze::stop()
{ _targetUnit->_unitAttributes |= Unit::Attributes::INPUT; }


DuelInvulnerability::DuelInvulnerability(const std::shared_ptr<Unit>& targetUnit, float duration)
: Effect(targetUnit, duration)
{ }


void
DuelInvulnerability::start()
{ _targetUnit->_unitAttributes &= ~(Unit::Attributes::DUELABLE); }


void
DuelInvulnerability::stop()
{ _targetUnit->_unitAttributes |= Unit::Attributes::DUELABLE; }


RespawnInvulnerability::RespawnInvulnerability(const std::shared_ptr<Unit>& targetUnit, float duration)
: Effect(targetUnit, duration)
{ }


void
RespawnInvulnerability::start()
{
    _targetUnit->_objAttributes &= ~(GameObject::Attributes::PASSABLE);
    _targetUnit->_unitAttributes &= ~(Unit::Attributes::DUELABLE);
}


void
RespawnInvulnerability::stop()
{
    _targetUnit->_objAttributes |= GameObject::Attributes::PASSABLE;
    _targetUnit->_unitAttributes |= Unit::Attributes::DUELABLE;
}
