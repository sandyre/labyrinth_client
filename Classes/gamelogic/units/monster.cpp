//
//  monster.cpp
//  labyrinth
//
//  Created by Aleksandr Borzikh on 17.01.17.
//
//

#include "monster.hpp"

#include "cocos2d.h"

Monster::Monster()
{
    m_eUnitType = Unit::Type::MONSTER;
    m_sName = "Default monster";
    
    m_nBaseDamage = m_nActualDamage = 10;
    m_nMHealth = m_nHealth = 50;
}

Monster *
Monster::create(const std::string& filename)
{
    Monster * pMonster = new Monster();
    
    if(pMonster->initWithFile(filename))
    {
        pMonster->autorelease();
        return pMonster;
    }
    
    CC_SAFE_DELETE(pMonster);
    return nullptr;
}
