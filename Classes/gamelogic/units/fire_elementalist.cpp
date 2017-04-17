//
//  fire_elementalist.cpp
//  labyrinth
//
//  Created by Aleksandr Borzikh on 03.03.17.
//
//

#include "fire_elementalist.hpp"

#include "gsnet_generated.h"

FireElementalist::FireElementalist()
{
    m_eHero = Hero::Type::FIRE_ELEMENTALIST;
    m_nMoveSpeed = 0.1;
    m_nMHealth = m_nHealth = 60;
    m_nBaseDamage = m_nActualDamage = 10;
    
    m_nSpell1CD = 30.0;
    m_nSpell1ACD = 0.0;
}

FireElementalist *
FireElementalist::create(const std::string& filename)
{
    FireElementalist * pAir = new FireElementalist();
    
    if(pAir->initWithFile(filename))
    {
        pAir->autorelease();
        return pAir;
    }
    
    CC_SAFE_DELETE(pAir);
    return nullptr;
}
