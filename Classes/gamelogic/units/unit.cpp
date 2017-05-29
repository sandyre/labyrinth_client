//
//  unit.cpp
//  labyrinth
//
//  Created by Aleksandr Borzikh on 07.03.17.
//
//

#include "unit.hpp"

#include "../gameworld.hpp"
#include "../../gsnet_generated.h"
#include "../effect.hpp"

#include <cocos2d.h>

Unit::Unit() :
m_eUnitType(Unit::Type::UNDEFINED),
m_eState(Unit::State::UNDEFINED),
m_eOrientation(Unit::Orientation::DOWN),
m_nUnitAttributes(0),
m_sName("Unit"),
m_nBaseDamage(10),
m_nActualDamage(10),
m_nHealth(50),
m_nMHealth(50),
m_nArmor(2),
m_nMResistance(2),
m_nMoveSpeed(2.0),
m_pDuelTarget(nullptr)
{
    m_eObjType = GameObject::Type::UNIT;
    m_eState = Unit::State::WALKING;
    m_nObjAttributes |= GameObject::Attributes::MOVABLE;
    m_nObjAttributes |= GameObject::Attributes::DAMAGABLE;
    
    m_nUnitAttributes |= Unit::Attributes::INPUT;
    m_nUnitAttributes |= Unit::Attributes::ATTACK;
    m_nUnitAttributes |= Unit::Attributes::DUELABLE;
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

int16_t
Unit::GetArmor() const
{
    return m_nArmor;
}

int16_t
Unit::GetMagicResistance() const
{
    return m_nMResistance;
}

Unit * const
Unit::GetDuelTarget() const
{
    return m_pDuelTarget;
}

std::vector<Item*>&
Unit::GetInventory()
{
    return m_aInventory;
}

void
Unit::ApplyEffect(Effect * effect)
{
    effect->start();
    m_aAppliedEffects.push_back(effect);
}

void
Unit::update(float delta)
{
    UpdateCDs(delta);
    for(auto effect : m_aAppliedEffects)
    {
        effect->update(delta);
        if(effect->GetState() == Effect::State::OVER)
        {
            effect->stop();
        }
    }
    m_aAppliedEffects.erase(std::remove_if(m_aAppliedEffects.begin(),
                                           m_aAppliedEffects.end(),
                                           [this](Effect * eff)
                                           {
                                               if(eff->GetState() == Effect::State::OVER)
                                               {
                                                   delete eff;
                                                   return true;
                                               }
                                               return false;
                                           }),
                            m_aAppliedEffects.end());
}

void
Unit::UpdateCDs(float delta)
{
    for(auto& cd : m_aSpellCDs)
    {
        if(std::get<0>(cd) == false)
        {
            std::get<1>(cd) -= delta;
            
            if(std::get<1>(cd) <= 0.0f)
            {
                std::get<0>(cd) = true;
                std::get<1>(cd) = 0.0f;
            }
        }
    }
}
/*
 *
 * Actions request
 *
 */
void
Unit::RequestMove(MoveDirection dir)
{
        // detect if player is already moving
    auto action = this->getActionByTag(5);
    if(action != nullptr)
        return;
    
    flatbuffers::FlatBufferBuilder builder;
    auto move = GameEvent::CreateCLActionMove(builder,
                                              this->GetUID(),
                                              (char)dir);
    auto event = GameEvent::CreateMessage(builder,
                                          GameEvent::Events_CLActionMove,
                                          move.Union());
    builder.Finish(event);
    
    m_poGameWorld->m_aOutEvents.emplace(builder.GetBufferPointer(),
                                        builder.GetBufferPointer() + builder.GetSize());
}

void
Unit::RequestTakeItem(Item * item)
{
        // check that item can be taken
    if(item->GetCarrierID() == 0)
    {
        flatbuffers::FlatBufferBuilder builder;
        auto take = GameEvent::CreateCLActionItem(builder,
                                                  this->GetUID(),
                                                  item->GetUID(),
                                                  GameEvent::ActionItemType_TAKE);
        auto event = GameEvent::CreateMessage(builder,
                                              GameEvent::Events_CLActionItem,
                                              take.Union());
        builder.Finish(event);
        
        m_poGameWorld->m_aOutEvents.emplace(builder.GetBufferPointer(),
                                            builder.GetBufferPointer() + builder.GetSize());
    }
}

void
Unit::RequestStartDuel(Unit * enemy)
{
    flatbuffers::FlatBufferBuilder builder;
    auto take = GameEvent::CreateCLActionDuel(builder,
                                              this->GetUID(),
                                              enemy->GetUID(),
                                              GameEvent::ActionDuelType_STARTED);
    auto event = GameEvent::CreateMessage(builder,
                                          GameEvent::Events_CLActionDuel,
                                          take.Union());
    builder.Finish(event);
    
    m_poGameWorld->m_aOutEvents.emplace(builder.GetBufferPointer(),
                                        builder.GetBufferPointer() + builder.GetSize());
}

/*
 *
 * Animations
 *
 */

void
Unit::Spawn(cocos2d::Vec2 log_pos)
{
    using Attributes = GameObject::Attributes;
    m_eState = Unit::State::WALKING;
    m_nObjAttributes = Attributes::DAMAGABLE |
                    Attributes::VISIBLE |
                    Attributes::MOVABLE;
    m_nUnitAttributes = Unit::Attributes::ATTACK |
    Unit::Attributes::DUELABLE |
    Unit::Attributes::INPUT;
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

    // TODO: seems like it doesn't work
void
Unit::DropItem(int32_t index)
{
    auto item_iter = m_aInventory.begin() + index;

        // make item visible and set its coords
    (*item_iter)->SetCarrierID(0);
    (*item_iter)->SetLogicalPosition(this->GetLogicalPosition());
    (*item_iter)->setPosition(LOG_TO_PHYS_COORD((*item_iter)->GetLogicalPosition(),
                                                (*item_iter)->getContentSize()));
    (*item_iter)->setVisible(true);
    
    // delete item from inventory
    m_aInventory.erase(item_iter);
}

void
Unit::TakeDamage(int16_t damage,
                 Unit::DamageType dmg_type,
                 Unit * damage_dealer)
{
    int16_t damage_taken = damage;
    if(dmg_type == Unit::DamageType::PHYSICAL)
        damage_taken -= m_nArmor;
    else if(dmg_type == Unit::DamageType::MAGICAL)
        damage_taken -= m_nMResistance;
    
    m_nHealth -= damage_taken;
    
    auto hp_text = cocos2d::Label::create(cocos2d::StringUtils::format("-%d", damage_taken),
                                          "fonts/alagard.ttf",
                                          12);
    hp_text->setTextColor(dmg_type == Unit::DamageType::PHYSICAL ? cocos2d::Color4B::WHITE : cocos2d::Color4B::MAGENTA);
    
    auto center_pos = this->getContentSize() / 2;
    hp_text->setPosition(center_pos);
    this->addChild(hp_text);
    
    auto moveup = cocos2d::MoveBy::create(1,
                                          cocos2d::Vec2(0, center_pos.height));
    auto fadeout = cocos2d::FadeOut::create(0.5f);
    auto seq = cocos2d::Sequence::create(moveup,
                                         fadeout,
                                         cocos2d::RemoveSelf::create(),
                                         nullptr);
    hp_text->runAction(seq);
}

void
Unit::Die(Unit * killer)
{
    m_eState = Unit::State::DEAD;
    m_nHealth = 0;
    m_nObjAttributes = GameObject::Attributes::PASSABLE;
    m_nUnitAttributes = 0;
    
    if(m_pDuelTarget != nullptr)
        EndDuel();
    
        // drop items
    while(!m_aInventory.empty())
    {
        this->DropItem(0);
    }
    
        // remove all effects
    for(auto effect : m_aAppliedEffects)
    {
        effect->stop();
    }
    
        // animation
    auto fadeOut = cocos2d::FadeOut::create(0.5);
    this->runAction(fadeOut);
}

uint32_t
Unit::GetUnitAttributes() const
{
    return m_nUnitAttributes;
}

void
Unit::Move(const GameEvent::SVActionMove* mov)
{
    Orientation new_orient = (Orientation)mov->mov_dir();
    MoveDirection mov_dir = (MoveDirection)mov->mov_dir();
    cocos2d::Vec2 new_pos = m_stLogPosition;
    if(mov_dir == MoveDirection::UP)
    {
        ++new_pos.y;
    }
    else if(mov_dir == MoveDirection::DOWN)
    {
        --new_pos.y;
    }
    else if(mov_dir == MoveDirection::LEFT)
    {
        --new_pos.x;
    }
    else if(mov_dir == MoveDirection::RIGHT)
    {
        ++new_pos.x;
    }
    
        // change orientation properly
    if(m_eOrientation != new_orient)
    {
        if(new_orient == Orientation::LEFT)
        {
            this->setFlippedX(true);
        }
        else if(new_orient == Orientation::RIGHT)
        {
            this->setFlippedX(false);
        }
    }
    
    m_eOrientation = new_orient;
    
    if(new_pos != cocos2d::Vec2(mov->x(),
                                mov->y()))
    {
        new_pos = cocos2d::Vec2(mov->x(),
                                mov->y());
    }
    
    m_stLogPosition = new_pos;
    
        // animation
    auto moveTo = cocos2d::MoveTo::create(1.0/m_nMoveSpeed,
                                          LOG_TO_PHYS_COORD(new_pos, this->getContentSize()));
    moveTo->setTag(5);
    this->runAction(moveTo);
}

void
Unit::TakeItem(Item * item)
{
    item->SetCarrierID(this->GetUID());
    item->setVisible(false);
    m_aInventory.push_back(item);
}

void
Unit::StartDuel(Unit * enemy)
{
    m_eState = Unit::State::DUEL;
    m_nUnitAttributes &= ~(Unit::Attributes::DUELABLE);
    
    m_pDuelTarget = enemy;
}

void
Unit::EndDuel()
{
    m_eState = Unit::State::WALKING;
    m_nUnitAttributes |= Unit::Attributes::DUELABLE;
    
    m_pDuelTarget = nullptr;
}
