//
//  mage.cpp
//  labyrinth
//
//  Created by Aleksandr Borzikh on 03.03.17.
//
//

#include "mage.hpp"

#include "../gameworld.hpp"
#include "gsnet_generated.h"

Mage::Mage()
{
    m_eHero = Hero::Type::MAGE;
    m_nMoveSpeed = 0.33;
    m_nMHealth = m_nHealth = 50;
    m_nBaseDamage = m_nActualDamage = 15;
    m_nArmor = 0;
    
    m_nSpell1CD = 30.0;
    m_nSpell1ACD = 0.0;
    
    m_nDashDuration = 10.0;
    m_nDashADuration = 0.0;
    m_bDashing = false;
}

Mage *
Mage::create(const std::string& filename)
{
    Mage * pMage = new Mage();
    
    if(pMage->initWithFile(filename))
    {
        pMage->autorelease();
        return pMage;
    }
    
    CC_SAFE_DELETE(pMage);
    return nullptr;
}

void
Mage::RequestSpellCast1()
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
Mage::SpellCast1()
{
    m_nSpell1ACD = m_nSpell1CD;
    m_nDashADuration = m_nDashDuration;
    m_bDashing = true;
    
    m_nMoveSpeed = 0.001;
}

void
Mage::update(float delta)
{
    Hero::update(delta);
    
    if(m_bDashing &&
       m_nDashADuration > 0.0)
    {
        m_nDashADuration -= delta;
    }
    else if(m_bDashing &&
            m_nDashADuration < 0.0)
    {
        m_bDashing = false;
        m_nMoveSpeed = 0.3;
    }
}
