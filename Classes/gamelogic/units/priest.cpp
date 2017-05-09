//
//  priest.cpp
//  labyrinth
//
//  Created by Aleksandr Borzikh on 03.03.17.
//
//

#include "priest.hpp"

#include "gsnet_generated.h"

Priest::Priest()
{
    m_eHero = Hero::Type::PRIEST;
    m_nMoveSpeed = 0.33;
    m_nMHealth = m_nHealth = 50;
    m_nBaseDamage = m_nActualDamage = 10;
    m_nArmor = 4;
    
    m_nSpell1CD = 30.0;
    m_nSpell1ACD = 0.0;
    
    m_nRegenInterval = 2.0;
    m_nRegenTimer = 0.0;
    m_nRegenAmount = 1;
}

Priest *
Priest::create(const std::string& filename)
{
    Priest * pPriest = new Priest();
    
    if(pPriest->initWithFile(filename))
    {
        pPriest->autorelease();
        return pPriest;
    }
    
    CC_SAFE_DELETE(pPriest);
    return nullptr;
}

void
Priest::update(float delta)
{
    Hero::update(delta);
    
        // passive regen works only in duel mode
    if(m_eState == Unit::State::DUEL)
    {
        m_nRegenTimer -= delta;
        if(m_nRegenTimer < 0.0 &&
           m_nHealth <= m_nMHealth)
        {
            m_nHealth += m_nRegenAmount;
            if(m_nHealth > m_nMHealth)
                m_nHealth = m_nMHealth;
            m_nRegenTimer = m_nRegenInterval;
        }
    }
}
