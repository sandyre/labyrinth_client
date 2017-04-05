//
//  player.cpp
//  labyrinth
//
//  Created by Aleksandr Borzikh on 17.01.17.
//
//

#include "hero.hpp"
#include "gsnet_generated.h"

Hero::Hero() :
m_eHero(Hero::Type::NO_HERO),
m_nSpell1CD(0.0),
m_nSpell1ACD(0.0)
{
    m_eUnitType = Unit::Type::HERO;
}

Hero::Type
Hero::GetHero() const
{
    return m_eHero;
}

void
Hero::TakeItem(Item * item)
{
    m_aInventory.push_back(item);
}

std::vector<Item*>&
Hero::GetInventory()
{
    return m_aInventory;
}

std::vector<char>
Hero::EventMove(cocos2d::Vec2 pos)
{
    std::vector<char> event;
    
    if(pos == GetLogicalPosition())
        return event;
    
    if(!this->getNumberOfRunningActions())
    {
        flatbuffers::FlatBufferBuilder builder;
        auto cl_mov = GameEvent::CreateCLActionMove(builder,
                                                    this->GetUID(),
                                                    GameEvent::ActionMoveTarget_PLAYER,
                                                    pos.x,
                                                    pos.y);
        auto gs_event = GameEvent::CreateMessage(builder,
                                                 GameEvent::Events_CLActionMove,
                                                 cl_mov.Union());
        builder.Finish(gs_event);
        
        event.assign(builder.GetBufferPointer(),
                     builder.GetBufferPointer() + builder.GetSize());
    }
    
    return event;
}

std::vector<char>
Hero::EventItemTake(uint32_t itemid)
{
    flatbuffers::FlatBufferBuilder builder;
    auto cl_take = GameEvent::CreateCLActionItem(builder,
                                                 this->GetUID(),
                                                 itemid,
                                                 GameEvent::ActionItemType_TAKE);
    auto gs_event = GameEvent::CreateMessage(builder,
                                             GameEvent::Events_CLActionItem,
                                             cl_take.Union());
    builder.Finish(gs_event);
    
    return std::vector<char>(builder.GetBufferPointer(),
                             builder.GetBufferPointer() + builder.GetSize());
}

std::vector<char>
Hero::EventDuelAttack()
{
    flatbuffers::FlatBufferBuilder builder;
    auto cl_attack = GameEvent::CreateCLActionDuel(builder,
                                                   this->GetUID(),
                                                   GameEvent::ActionDuelTarget_PLAYER,
                                                   m_pDuelTarget->GetUID(),
                                                   m_pDuelTarget->GetUnitType() == Unit::Type::HERO ?
                                                       GameEvent::ActionDuelTarget_PLAYER :
                                                       GameEvent::ActionDuelTarget_MONSTER,
                                                   GameEvent::ActionDuelType_ATTACK,
                                                   m_nActualDamage);
    auto sv_event = GameEvent::CreateMessage(builder,
                                             GameEvent::Events_CLActionDuel,
                                             cl_attack.Union());
    builder.Finish(sv_event);
    
    return std::vector<char>(builder.GetBufferPointer(),
                             builder.GetBufferPointer() + builder.GetSize());
}

std::vector<char>
Hero::EventDuelStart(uint32_t id)
{
    flatbuffers::FlatBufferBuilder builder;
    auto cl_duel = GameEvent::CreateCLActionDuel(builder,
                                                 this->GetUID(),
                                                 GameEvent::ActionDuelTarget_PLAYER,
                                                 id,
                                                 GameEvent::ActionDuelTarget_PLAYER,
                                                 GameEvent::ActionDuelType_STARTED);
    auto gs_event = GameEvent::CreateMessage(builder,
                                             GameEvent::Events_CLActionDuel,
                                             cl_duel.Union());
    builder.Finish(gs_event);
    
    return std::vector<char>(builder.GetBufferPointer(),
                             builder.GetBufferPointer() + builder.GetSize());
}

void
Hero::UpdateStats()
{
    int new_dmg = m_nBaseDamage;
    for(auto item : m_aInventory)
    {
        if(item->GetType() == Item::Type::SWORD)
            new_dmg += 6;
    }
    m_nActualDamage = new_dmg;
}

void
Hero::update(float delta)
{
    UpdateCDs(delta);
}

void
Hero::UpdateCDs(float delta)
{
    if(m_nSpell1ACD > 0.0)
    {
        m_nSpell1ACD -= delta;
    }
    else
    {
        m_nSpell1ACD = 0.0;
    }
}

bool
Hero::isSpellCast1Ready() const
{
    if(m_nSpell1ACD == 0.0)
        return true;
    return false;
}

void
Hero::AnimationSpell1()
{
    
}

float
Hero::GetSpell1ACD() const
{
    return m_nSpell1ACD;
}

void
Hero::Turn(Hero::Orientation orient)
{
    m_eOrientation = orient;
    
    switch(orient)
    {
        case Orientation::UP:
        {
            this->setTexture("res/units/player_up.png");
            break;
        }
        case Orientation::DOWN:
        {
            this->setTexture("res/units/player_down.png");
            break;
        }
        case Orientation::LEFT:
        {
            this->setTexture("res/units/player_left.png");
            break;
        }
        case Orientation::RIGHT:
        {
            this->setTexture("res/units/player_right.png");
            break;
        }
    }
}
