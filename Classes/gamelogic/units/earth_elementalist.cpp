//
//  earth_elementalist.cpp
//  labyrinth
//
//  Created by Aleksandr Borzikh on 03.03.17.
//
//

#include "earth_elementalist.hpp"

#include "gsnet_generated.h"

EarthElementalist::EarthElementalist()
{
    m_eHero = Hero::Type::EARTH_ELEMENTALIST;
    m_nMoveSpeed = 0.4;
    m_nMHealth = m_nHealth = 100;
    m_nBaseDamage = m_nActualDamage = 8;
    
    m_nSpell1CD = 3.0;
    m_nSpell1ACD = 0.0;
}

EarthElementalist *
EarthElementalist::create(const std::string& filename)
{
    EarthElementalist * pAir = new EarthElementalist();
    
    if(pAir->initWithFile(filename))
    {
        pAir->autorelease();
        return pAir;
    }
    
    CC_SAFE_DELETE(pAir);
    return nullptr;
}

void
EarthElementalist::SpellCast1()
{
    m_nSpell1ACD = m_nSpell1CD;
}

void
EarthElementalist::update(float delta)
{
    Hero::UpdateCDs(delta);
}
