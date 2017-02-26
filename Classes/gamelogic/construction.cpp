//
//  construction.cpp
//  labyrinth
//
//  Created by Aleksandr Borzikh on 21.01.17.
//
//

#include "construction.hpp"

/*
 * <Construction class implementation>
 */

Construction::Construction()
{
    m_eObjType = GameObject::Type::CONSTRUCTION;
}

Construction::Type
Construction::GetType() const
{
    return m_eType;
}

/*
 * </Construction class implementation>
 */

/*
 * <Door class implementation>
 */

Door::Door()
{
    m_eType = Construction::Type::DOOR;
}

Door *
Door::create(const std::string& filename)
{
    Door * pDoor = new Door();
    
    if(pDoor->initWithFile(filename))
    {
        pDoor->autorelease();
        return pDoor;
    }
    
    CC_SAFE_DELETE(pDoor);
    return nullptr;
}

/*
 * </Door class implementation>
 */

/*
 * <Graveyard class implementation>
 */

Graveyard::Graveyard()
{
    m_eType = Construction::Type::GRAVEYARD;
}

Graveyard *
Graveyard::create(const std::string& filename)
{
    Graveyard * pGrave = new Graveyard();
    
    if(pGrave->initWithFile(filename))
    {
        pGrave->autorelease();
        return pGrave;
    }
    
    CC_SAFE_DELETE(pGrave);
    return nullptr;
}

/*
 * </Graveyard class implementation>
 */

/*
 * <Swamp class implementation>
 */

Swamp::Swamp()
{
    m_eType = Construction::Type::SWAMP;
}

Swamp *
Swamp::create(const std::string& filename)
{
    Swamp * pSwamp = new Swamp();
    
    if(pSwamp->initWithFile(filename))
    {
        pSwamp->autorelease();
        return pSwamp;
    }
    
    CC_SAFE_DELETE(pSwamp);
    return nullptr;
}

/*
 * </Swamp class implementation>
 */
