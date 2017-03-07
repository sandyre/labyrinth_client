//
//  unit.cpp
//  labyrinth
//
//  Created by Aleksandr Borzikh on 07.03.17.
//
//

#include "unit.hpp"

Unit::Unit() :
m_eUnitType(Unit::Type::UNDEFINED),
m_eState(Unit::State::UNDEFINED),
m_eOrientation(Unit::Orientation::DOWN),
m_nStatus(0),
m_sName("Unit"),
m_nBaseDamage(0),
m_nActualDamage(0),
m_nHealth(0),
m_nMHealth(0),
m_nMoveSpeed(0.5),
m_pDuelTarget(nullptr)
{
    m_eObjType = GameObject::Type::UNIT;
    this->scheduleUpdate();
}

Unit::Type
Unit::GetUnitType() const
{
    return m_eUnitType;
}

Unit::State
Unit::GetState() const
{
    return m_eState;
}

Unit::Orientation
Unit::GetOrientation() const
{
    return m_eOrientation;
}

uint32_t
Unit::GetStatus() const
{
    return m_nStatus;
}

std::string
Unit::GetName() const
{
    return m_sName;
}

void
Unit::SetName(const std::string& name)
{
    m_sName = name;
}

int16_t
Unit::GetDamage() const
{
    return m_nActualDamage;
}

int16_t
Unit::GetHealth() const
{
    return m_nHealth;
}

int16_t
Unit::GetMaxHealth() const
{
    return m_nMHealth;
}

Unit * const
Unit::GetDuelTarget() const
{
    return m_pDuelTarget;
}

/*
 *
 * Animations
 *
 */

void
Unit::Spawn(cocos2d::Vec2 log_pos)
{
    m_eState = Unit::State::WALKING;
    m_nStatus = Unit::Status::MOVABLE |
                Unit::Status::DUELABLE;
    m_nHealth = m_nMHealth;
    
    m_stLogPosition = log_pos;
    this->setPosition(LOG_TO_PHYS_COORD(log_pos,
                                        this->getContentSize()));
    
        // animation
    auto fadeIn = cocos2d::FadeIn::create(0.5);
    this->runAction(fadeIn);
}

void
Unit::Respawn(cocos2d::Vec2 log_pos)
{
    this->Spawn(log_pos);
}

void
Unit::Die()
{
    m_eState = Unit::State::DEAD;
    m_nStatus = 0;
    m_nHealth = 0;
    
        // animation
    auto fadeOut = cocos2d::FadeOut::create(0.5);
    this->runAction(fadeOut);
}

void
Unit::Move(cocos2d::Vec2 log_pos)
{
    m_stLogPosition = log_pos;
        // animation
    auto moveTo = cocos2d::MoveTo::create(m_nMoveSpeed,
                                          LOG_TO_PHYS_COORD(log_pos, this->getContentSize()));
    this->runAction(moveTo);
}

void
Unit::Attack()
{
    m_pDuelTarget->TakeDamage(m_nActualDamage);
}

void
Unit::TakeDamage(int16_t dmg)
{
    m_nHealth -= dmg;
}

void
Unit::StartDuel(Unit * enemy)
{
    m_eState = Unit::State::DUEL;
    m_nStatus ^= Unit::Status::DUELABLE;
    
    m_pDuelTarget = enemy;
}

void
Unit::EndDuel()
{
    m_eState = Unit::State::WALKING;
    m_nStatus ^= Unit::Status::DUELABLE;
    
    m_pDuelTarget = nullptr;
}
