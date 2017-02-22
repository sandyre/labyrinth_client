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

/*
 * </Swamp class implementation>
 */
