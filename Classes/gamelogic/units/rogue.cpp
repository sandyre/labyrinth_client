//
//  rogue.cpp
//  labyrinth
//
//  Created by Aleksandr Borzikh on 03.03.17.
//
//

#include "rogue.hpp"

#include "../gameworld.hpp"
#include "gsnet_generated.h"

Rogue::Rogue()
{
    m_eHero = Hero::Type::ROGUE;
    m_nMoveSpeed = 0.125;
    m_nMHealth = m_nHealth = 50;
    m_nBaseDamage = m_nActualDamage = 8;
    m_nArmor = 2;
    
    m_nSpell1CD = 30.0;
    m_nSpell1ACD = 0.0;
    
    m_nInvisDuration = 10.0;
    m_nInvisADuration = 0.0;
    m_bInvisible = false;
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

void
Rogue::RequestSpellCast1()
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
Rogue::SpellCast1()
{
    m_nSpell1ACD = m_nSpell1CD;
    m_bInvisible = true;
    m_nAttributes ^= GameObject::Attributes::DUELABLE;
    
    m_nInvisADuration = m_nInvisDuration;
    
    auto fadeOut = cocos2d::FadeOut::create(0.5);
    this->runAction(fadeOut);
}

void
Rogue::TakeItem(Item * item)
{
    Unit::TakeItem(item);
    
    if(m_bInvisible)
    {
        m_bInvisible = false;
        m_nInvisADuration = 0.0;
        m_nAttributes |= GameObject::Attributes::DUELABLE;
        auto fadeIn = cocos2d::FadeIn::create(0.5);
        this->runAction(fadeIn);
    }
}

void
Rogue::update(float delta)
{
    Hero::update(delta);
    
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
        m_nAttributes |= GameObject::Attributes::DUELABLE;
        
        auto fadeIn = cocos2d::FadeIn::create(0.5);
        this->runAction(fadeIn);
    }
}
