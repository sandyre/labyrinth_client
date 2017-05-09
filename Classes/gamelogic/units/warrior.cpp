//
//  warrior.cpp
//  labyrinth
//
//  Created by Aleksandr Borzikh on 03.03.17.
//
//

#include "warrior.hpp"

#include "../gameworld.hpp"
#include "gsnet_generated.h"

Warrior::Warrior()
{
    m_eHero = Hero::Type::WARRIOR;
    m_nMoveSpeed = 0.4;
    m_nMHealth = m_nHealth = 50;
    m_nArmor = 6;
    m_nBaseDamage = m_nActualDamage = 10;
    
    m_nSpell1CD = 10.0;
    m_nSpell1ACD = 0.0;
    
    m_nDashingDuration = 3.0;
}

Warrior *
Warrior::create(const std::string& filename)
{
    Warrior * pWar = new Warrior();
    
    if(pWar->initWithFile(filename))
    {
        pWar->autorelease();
        return pWar;
    }
    
    CC_SAFE_DELETE(pWar);
    return nullptr;
}

void
Warrior::RequestSpellCast1()
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
Warrior::SpellCast1()
{
    m_nSpell1ACD = m_nSpell1CD;
    m_bDashing = true;
    
    m_nDashingADuration = m_nDashingDuration;
    m_nMoveSpeed = 0.1;
}

void
Warrior::update(float delta)
{
    Hero::update(delta);
    
    if(m_bDashing &&
       m_nDashingADuration > 0.0)
    {
        m_nDashingADuration -= delta;
    }
    else if(m_bDashing &&
            m_nDashingADuration < 0.0)
    {
        m_bDashing = false;
        m_nDashingADuration = 0.0;
        m_nMoveSpeed = 0.4; // make it default
    }
}
