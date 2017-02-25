//
//  mapblock.cpp
//  labyrinth
//
//  Created by Aleksandr Borzikh on 21.01.17.
//
//

#include "mapblock.hpp"

/*
 * <MapBlock class implementation>
 */

MapBlock::MapBlock() :
m_bDiscovered(false)
{
    m_eObjType = GameObject::Type::MAPBLOCK;
}

MapBlock::Type
MapBlock::GetType() const
{
    return m_eType;
}

bool
MapBlock::IsDiscovered() const
{
    return m_bDiscovered;
}

void
MapBlock::SetDiscovered(bool val)
{
    m_bDiscovered = val;
}

/*
 * </MapBlock class implementation>
 */

/*
 * <WallBlock class implementation>
 */

WallBlock::WallBlock()
{
    m_eType = MapBlock::Type::WALL;
}

/*
 * </WallBlock class implementation>
 */

/*
 * <NoBlock class implementation>
 */

NoBlock::NoBlock()
{
    m_eType = MapBlock::Type::NOBLOCK;
}

/*
 * </NoBlock class implementation>
 */

/*
 * <BorderBlock class implementation>
 */

BorderBlock::BorderBlock()
{
    m_eType = MapBlock::Type::BORDER;
}

/*
 * </BorderBlock class implementation>
 */
