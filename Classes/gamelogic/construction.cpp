//
//  construction.cpp
//  labyrinth
//
//  Created by Aleksandr Borzikh on 21.01.17.
//
//

#include "construction.hpp"

Construction::Construction()
{
    m_eObjType = GameObject::Type::CONSTRUCTION;
}

Construction::Type
Construction::GetType() const
{
    return m_eType;
}

Door::Door()
{
    m_eType = Construction::Type::DOOR;
    m_nObjAttributes |= GameObject::Attributes::PASSABLE;
}

Door *
Door::create(const std::string& filename)
{
    Door * pDoor = new Door();
    
    if(pDoor->initWithSpriteFrameName(filename))
    {
        pDoor->autorelease();
        return pDoor;
    }
    
    CC_SAFE_DELETE(pDoor);
    return nullptr;
}

Graveyard::Graveyard()
{
    m_eType = Construction::Type::GRAVEYARD;
    m_nObjAttributes |= GameObject::Attributes::PASSABLE;
}

Graveyard *
Graveyard::create(const std::string& filename)
{
    Graveyard * pGrave = new Graveyard();
    
    if(pGrave->initWithSpriteFrameName(filename))
    {
        pGrave->autorelease();
        return pGrave;
    }
    
    CC_SAFE_DELETE(pGrave);
    return nullptr;
}

Swamp::Swamp()
{
    m_eType = Construction::Type::SWAMP;
}

Swamp *
Swamp::create(const std::string& filename)
{
    Swamp * pSwamp = new Swamp();
    
    if(pSwamp->initWithSpriteFrameName(filename))
    {
        pSwamp->autorelease();
        return pSwamp;
    }
    
    CC_SAFE_DELETE(pSwamp);
    return nullptr;
}
