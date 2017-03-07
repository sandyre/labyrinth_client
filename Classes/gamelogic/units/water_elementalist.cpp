//
//  water_elementalist.cpp
//  labyrinth
//
//  Created by Aleksandr Borzikh on 03.03.17.
//
//

#include "water_elementalist.hpp"

#include "gsnet_generated.h"

WaterElementalist::WaterElementalist()
{
    m_eHero = Hero::Type::WATER_ELEMENTALIST;
    m_nMoveSpeed = 0.3;
    m_nMHealth = m_nHealth = 75;
    m_nBaseDamage = m_nActualDamage = 8;
    
    m_nSpell1CD = 30.0;
    m_nSpell1ACD = 0.0;
    
    m_nDashDuration = 10.0;
    m_nDashADuration = 0.0;
    m_bDashing = false;
}

WaterElementalist *
WaterElementalist::create(const std::string& filename)
{
    WaterElementalist * pAir = new WaterElementalist();
    
    if(pAir->initWithFile(filename))
    {
        pAir->autorelease();
        return pAir;
    }
    
    CC_SAFE_DELETE(pAir);
    return nullptr;
}

std::vector<char>
WaterElementalist::EventSpellCast1()
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
    
    return std::vector<char>(builder.GetBufferPointer(),
                             builder.GetBufferPointer() + builder.GetSize());
}

void
WaterElementalist::SpellCast1()
{
    m_nSpell1ACD = m_nSpell1CD;
    m_nDashADuration = m_nDashDuration;
    m_bDashing = true;
    
    m_nMoveSpeed = 0.001;
}

void
WaterElementalist::update(float delta)
{
    Hero::UpdateCDs(delta);
    
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
