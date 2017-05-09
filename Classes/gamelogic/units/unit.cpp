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

#include <cocos2d.h>

Unit::Unit() :
m_eUnitType(Unit::Type::UNDEFINED),
m_eState(Unit::State::UNDEFINED),
m_eOrientation(Unit::Orientation::DOWN),
m_sName("Unit"),
m_nBaseDamage(0),
m_nActualDamage(0),
m_nHealth(0),
m_nMHealth(0),
m_nArmor(0),
m_nMoveSpeed(0.5),
m_pDuelTarget(nullptr)
{
    m_eObjType = GameObject::Type::UNIT;
    m_eState = Unit::State::WALKING;
    m_nAttributes |= GameObject::Attributes::MOVABLE;
    m_nAttributes |= GameObject::Attributes::DUELABLE;
    m_nAttributes |= GameObject::Attributes::DAMAGABLE;
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
Unit::AddInputEvent(InputEvent event)
{
    m_aInputEvents.push(event);
}

/*
 *
 * Actions request
 *
 */
void
Unit::RequestMove(cocos2d::Vec2 pos)
{
        // check that path is clear
    for(auto object : m_poGameWorld->m_apoObjects)
    {
        if(object->GetLogicalPosition() == pos &&
           !(object->GetAttributes() & GameObject::Attributes::PASSABLE))
        {
                // unpassable object
            return;
        }
    }
    
        // detect if player is already moving
    auto action = this->getActionByTag(5);
    if(action != nullptr)
        return;
    
    flatbuffers::FlatBufferBuilder builder;
    auto move = GameEvent::CreateCLActionMove(builder,
                                              this->GetUID(),
                                              pos.x,
                                              pos.y);
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

void
Unit::RequestAttack()
{
    flatbuffers::FlatBufferBuilder builder;
    auto atk = GameEvent::CreateCLActionDuel(builder,
                                             this->GetUID(),
                                             m_pDuelTarget->GetUID(),
                                             GameEvent::ActionDuelType_ATTACK);
    auto msg = GameEvent::CreateMessage(builder,
                                        GameEvent::Events_CLActionDuel,
                                        atk.Union());
    builder.Finish(msg);
    
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
    m_nAttributes = Attributes::DAMAGABLE |
                    Attributes::DUELABLE |
                    Attributes::VISIBLE |
                    Attributes::MOVABLE;
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
Unit::Die()
{
    m_eState = Unit::State::DEAD;
    m_nHealth = 0;
    m_nAttributes = GameObject::Attributes::PASSABLE;
    
        // drop items
    while(!m_aInventory.empty())
    {
        this->DropItem(0);
    }
    
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
Unit::Attack()
{
    m_pDuelTarget->TakeDamage(m_nActualDamage);
}

void
Unit::TakeDamage(int16_t dmg)
{
    m_nHealth -= (dmg - m_nArmor);
    
    auto hp_text = cocos2d::Label::create(cocos2d::StringUtils::format("-%d", (dmg - m_nArmor)),
                                          "fonts/alagard.ttf",
                                          12);
    hp_text->setTextColor(cocos2d::Color4B::RED);
    
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
Unit::StartDuel(Unit * enemy)
{
    m_eState = Unit::State::DUEL;
    m_nAttributes ^= GameObject::Attributes::DUELABLE;
    
    m_pDuelTarget = enemy;
}

void
Unit::EndDuel()
{
    m_eState = Unit::State::WALKING;
    m_nAttributes ^= GameObject::Attributes::DUELABLE;
    
    m_pDuelTarget = nullptr;
}
