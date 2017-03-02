//
//  player.cpp
//  labyrinth
//
//  Created by Aleksandr Borzikh on 17.01.17.
//
//

#include "player.hpp"
#include "gsnet_generated.h"

Player::Player() :
m_eHero(Hero::UNDEFINED),
m_eState(Player::State::WALKING),
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

Hero
Player::GetHero() const
{
    return m_eHero;
}

Player::State
Player::GetState() const
{
    return m_eState;
}

void
Player::SetState(Player::State state)
{
    m_eState = state;
}

std::string
Player::GetNickname() const
{
    return m_sNickname;
}

void
Player::SetNickname(const std::string& name)
{
    m_sNickname = name;
}

uint32_t
Player::GetDuelTargetID() const
{
    return m_nDTargetID;
}

void
Player::SetDuelTargetID(uint32_t id)
{
    m_nDTargetID = id;
}

uint16_t
Player::GetHealth() const
{
    return m_nHealth;
}

void
Player::SetHealth(uint16_t hp)
{
    m_nHealth = hp;
}

uint16_t
Player::GetMaxHealth() const
{
    return m_nMHealth;
}

void
Player::SetMaxHealth(uint16_t hp)
{
    m_nMHealth = hp;
}

uint16_t
Player::GetDamage() const
{
    return m_nDamage;
}

std::vector<Item*>&
Player::GetInventory()
{
    return m_aInventory;
}

void
Player::AnimationSpawn()
{
    auto fadeIn = cocos2d::FadeIn::create(1);
    this->runAction(fadeIn);
}

void
Player::AnimationRespawn()
{
    auto fadeIn = cocos2d::FadeIn::create(1);
    this->runAction(fadeIn);
}

void
Player::AnimationDeath()
{
    auto fadeOut = cocos2d::FadeOut::create(1);
    this->runAction(fadeOut);
}

void
Player::AnimationMoveTo(cocos2d::Vec2 pos)
{
    auto moveTo = cocos2d::MoveTo::create(m_nMoveSpeed, pos);
    this->runAction(moveTo);
}

void
Player::AnimationSpell1()
{
    auto rotate = cocos2d::RotateBy::create(2.0, 360);
    this->runAction(rotate);
}

std::vector<char>
Player::EventMove(cocos2d::Vec2 pos)
{
    std::vector<char> event;
    
    if(!this->getNumberOfRunningActions())
    {
        flatbuffers::FlatBufferBuilder builder;
        auto cl_mov = GameEvent::CreateCLActionMove(builder,
                                                    this->GetUID(),
                                                    GameEvent::ActionMoveTarget_PLAYER,
                                                    pos.x,
                                                    pos.y);
        auto gs_event = GameEvent::CreateEvent(builder,
                                               GameEvent::Events_CLActionMove,
                                               cl_mov.Union());
        builder.Finish(gs_event);
        
        event.assign(builder.GetBufferPointer(),
                     builder.GetBufferPointer() + builder.GetSize());
    }
    
    return event;
}

std::vector<char>
Player::EventItemTake(uint32_t itemid)
{
    flatbuffers::FlatBufferBuilder builder;
    auto cl_take = GameEvent::CreateCLActionItem(builder,
                                                 this->GetUID(),
                                                 itemid,
                                                 GameEvent::ActionItemType_TAKE);
    auto gs_event = GameEvent::CreateEvent(builder,
                                           GameEvent::Events_CLActionItem,
                                           cl_take.Union());
    builder.Finish(gs_event);
    
    return std::vector<char>(builder.GetBufferPointer(),
                             builder.GetBufferPointer() + builder.GetSize());
}

std::vector<char>
Player::EventDuelAttack()
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
    auto sv_event = GameEvent::CreateEvent(builder,
                                           GameEvent::Events_CLActionDuel,
                                           cl_attack.Union());
    builder.Finish(sv_event);
    
    return std::vector<char>(builder.GetBufferPointer(),
                             builder.GetBufferPointer() + builder.GetSize());
}

std::vector<char>
Player::EventDuelStart(uint32_t id)
{
    flatbuffers::FlatBufferBuilder builder;
    auto cl_duel = GameEvent::CreateCLActionDuel(builder,
                                                 this->GetUID(),
                                                 GameEvent::ActionDuelTarget_PLAYER,
                                                 id,
                                                 GameEvent::ActionDuelTarget_PLAYER,
                                                 GameEvent::ActionDuelType_STARTED);
    auto gs_event = GameEvent::CreateEvent(builder,
                                           GameEvent::Events_CLActionDuel,
                                           cl_duel.Union());
    builder.Finish(gs_event);
    
    return std::vector<char>(builder.GetBufferPointer(),
                             builder.GetBufferPointer() + builder.GetSize());
}

void
Player::UpdateStats()
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
Player::update(float delta)
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
Player::isSpellCast1Ready() const
{
    if(m_nSpell1ACD == 0.0)
        return true;
    return false;
}

Rogue::Rogue()
{
    m_eHero = Hero::ROGUE;
    m_nMoveSpeed = 0.1;
    m_nDamage = 6;
    
    m_nSpell1CD = 5.0; // cooldown value
    m_nSpell1ACD = 0.0; // active cooldown
}

Rogue *
Rogue::create(const std::string& filename)
{
    Rogue * pRogue = new Rogue();
    
    if(pRogue->initWithFile(filename))
    {
        pRogue->autorelease();
        return pRogue;
    }
    
    CC_SAFE_DELETE(pRogue);
    return nullptr;
}

std::vector<char>
Rogue::EventSpellCast1()
{
        // cooldown begins
    m_nSpell1ACD = m_nSpell1CD;
    
    flatbuffers::FlatBufferBuilder builder;
    auto spell1 = GameEvent::CreateCLActionSpell(builder,
                                                 this->GetUID(),
                                                 1,
                                                 GameEvent::ActionSpellTarget_TARGET_POINT,
                                                 this->GetLogicalPosition().x,
                                                 this->GetLogicalPosition().y);
    auto event = GameEvent::CreateEvent(builder,
                                        GameEvent::Events_CLActionSpell,
                                        spell1.Union());
    builder.Finish(event);
    
    return std::vector<char>(builder.GetBufferPointer(),
                             builder.GetBufferPointer() + builder.GetSize());
}

Paladin::Paladin()
{
    m_eHero = Hero::PALADIN;
    m_nMoveSpeed = 0.4;
    m_nDamage = 4;
    
    m_nSpell1CD = 10.0;
    m_nSpell1ACD = 0.0;
}

Paladin *
Paladin::create(const std::string& filename)
{
    Paladin * pPaladin = new Paladin();
    
    if(pPaladin->initWithFile(filename))
    {
        pPaladin->autorelease();
        return pPaladin;
    }
    
    CC_SAFE_RELEASE(pPaladin);
    return nullptr;
}

std::vector<char>
Paladin::EventSpellCast1()
{
    m_nSpell1ACD = m_nSpell1CD;
    return std::vector<char>();
}
