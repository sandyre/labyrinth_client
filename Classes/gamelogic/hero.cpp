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
m_nStatus(Hero::Status::MOVABLE |
          Hero::Status::DUELABLE),
m_eHero(Hero::Type::UNDEFINED),
m_eState(Hero::State::WALKING),
m_eOrientation(Hero::Orientation::DOWN),
m_nHealth(0),
m_nMHealth(0),
m_nDTargetID(0),
m_nMoveSpeed(0.5),
m_nSpell1CD(0.0),
m_nSpell1ACD(0.0)
{
    m_eObjType = GameObject::Type::PLAYER;
    this->scheduleUpdate();
}

int32_t
Hero::GetStatus() const
{
    return m_nStatus;
}

Hero::Type
Hero::GetHero() const
{
    return m_eHero;
}

Hero::State
Hero::GetState() const
{
    return m_eState;
}

void
Hero::SetState(Hero::State state)
{
    m_eState = state;
}

std::string
Hero::GetNickname() const
{
    return m_sNickname;
}

void
Hero::SetNickname(const std::string& name)
{
    m_sNickname = name;
}

uint32_t
Hero::GetDuelTargetID() const
{
    return m_nDTargetID;
}

void
Hero::SetDuelTargetID(uint32_t id)
{
    m_nDTargetID = id;
}

void
Hero::AnimationTakeItem(Item * item)
{
    m_aInventory.push_back(item);
}

uint16_t
Hero::GetHealth() const
{
    return m_nHealth;
}

void
Hero::SetHealth(uint16_t hp)
{
    m_nHealth = hp;
}

uint16_t
Hero::GetMaxHealth() const
{
    return m_nMHealth;
}

void
Hero::SetMaxHealth(uint16_t hp)
{
    m_nMHealth = hp;
}

uint16_t
Hero::GetDamage() const
{
    return m_nDamage;
}

std::vector<Item*>&
Hero::GetInventory()
{
    return m_aInventory;
}

void
Hero::AnimationSpawn()
{
    auto fadeIn = cocos2d::FadeIn::create(1);
    this->runAction(fadeIn);
}

void
Hero::AnimationRespawn()
{
    auto fadeIn = cocos2d::FadeIn::create(1);
    this->runAction(fadeIn);
}

void
Hero::AnimationDeath()
{
    auto fadeOut = cocos2d::FadeOut::create(1);
    this->runAction(fadeOut);
}

void
Hero::AnimationMoveTo(cocos2d::Vec2 pos)
{
    auto moveTo = cocos2d::MoveTo::create(m_nMoveSpeed, pos);
    this->runAction(moveTo);
}

void
Hero::AnimationSpell1()
{
    
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
                                                   this->GetDuelTargetID(),
                                                   m_eState == State::DUEL_PLAYER ? GameEvent::ActionDuelTarget_PLAYER :
                                                                                    GameEvent::ActionDuelTarget_MONSTER,
                                                   GameEvent::ActionDuelType_ATTACK,
                                                   m_nDamage);
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
    int new_dmg = 6;
    for(auto item : m_aInventory)
    {
        if(item->GetType() == Item::Type::SWORD)
            new_dmg += 6;
    }
    m_nDamage = new_dmg;
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

Hero::Orientation
Hero::GetOrientation() const
{
    return m_eOrientation;
}

void
Hero::SetOrientation(Hero::Orientation orient)
{
    m_eOrientation = orient;
    
    switch(orient)
    {
        case Orientation::UP:
        {
            this->setTexture("res/player_up.png");
            break;
        }
        case Orientation::DOWN:
        {
            this->setTexture("res/player_down.png");
            break;
        }
        case Orientation::LEFT:
        {
            this->setTexture("res/player_left.png");
            break;
        }
        case Orientation::RIGHT:
        {
            this->setTexture("res/player_right.png");
            break;
        }
    }
}
