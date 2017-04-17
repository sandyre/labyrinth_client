//
//  air_elementalist.cpp
//  labyrinth
//
//  Created by Aleksandr Borzikh on 03.03.17.
//
//

#include "air_elementalist.hpp"

#include "../gameworld.hpp"
#include "gsnet_generated.h"

AirElementalist::AirElementalist()
{
    m_eHero = Hero::Type::AIR_ELEMENTALIST;
    m_nMoveSpeed = 0.1;
    m_nMHealth = m_nHealth = 75;
    m_nBaseDamage = m_nActualDamage = 6;
    
    m_nSpell1CD = 30.0;
    m_nSpell1ACD = 0.0;
    
    m_nInvisDuration = 10.0;
    m_nInvisADuration = 0.0;
    m_bInvisible = false;
}

AirElementalist *
AirElementalist::create(const std::string& filename)
{
    AirElementalist * pAir = new AirElementalist();
    
    if(pAir->initWithFile(filename))
    {
        pAir->autorelease();
        return pAir;
    }
    
    CC_SAFE_DELETE(pAir);
    return nullptr;
}

void
AirElementalist::RequestSpellCast1()
{
    flatbuffers::FlatBufferBuilder builder;
    auto spell1 = GameEvent::CreateCLActionSpell(builder,
                                                 this->GetUID(),
                                                 1,
                                                 GameEvent::ActionSpellTarget_TARGET_PLAYER,
                                                 this->GetUID());
    auto event = GameEvent::CreateMessage(builder,
                                          GameEvent::Events_CLActionSpell,
                                          spell1.Union());
    builder.Finish(event);
    
    m_poGameWorld->m_aOutEvents.emplace(builder.GetBufferPointer(),
                                        builder.GetBufferPointer() + builder.GetSize());
}

void
AirElementalist::SpellCast1()
{
    m_nSpell1ACD = m_nSpell1CD;
    m_bInvisible = true;
    m_nStatus ^= Hero::Status::DUELABLE;
    
    m_nInvisADuration = m_nInvisDuration;
    
    auto fadeOut = cocos2d::FadeOut::create(0.5);
    this->runAction(fadeOut);
}

void
AirElementalist::TakeItem(Item * item)
{
    m_aInventory.push_back(item);
    
    if(m_bInvisible)
    {
        m_bInvisible = false;
        m_nInvisADuration = 0.0;
        m_nStatus |= Hero::Status::DUELABLE;
        auto fadeIn = cocos2d::FadeIn::create(0.5);
        this->runAction(fadeIn);
    }
}

void
AirElementalist::update(float delta)
{
    Hero::process_input_events();
    Hero::UpdateCDs(delta);
    
    if(m_bInvisible &&
       m_nInvisADuration > 0.0)
    {
        m_nInvisADuration -= delta;
    }
    else if(m_bInvisible &&
            m_nInvisADuration < 0.0)
    {
            // make visible & duelable
        m_nInvisADuration = 0.0;
        m_bInvisible = false;
        m_nStatus |= Hero::Status::DUELABLE;
        
        auto fadeIn = cocos2d::FadeIn::create(0.5);
        this->runAction(fadeIn);
    }
}
