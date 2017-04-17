//
//  mapblock.cpp
//  labyrinth
//
//  Created by Aleksandr Borzikh on 21.01.17.
//
//

#include "mapblock.hpp"

MapBlock::MapBlock()
{
    m_eObjType = GameObject::Type::MAPBLOCK;
}

MapBlock::Type
MapBlock::GetType() const
{
    return m_eType;
}

NoBlock::NoBlock()
{
    m_eType = MapBlock::Type::NOBLOCK;
    m_nAttributes |= GameObject::Attributes::PASSABLE;
}

NoBlock *
NoBlock::create(const std::string& filename)
{
    NoBlock * pBlock = new NoBlock();
    
    if(pBlock->initWithFile(filename))
    {
        pBlock->autorelease();
        return pBlock;
    }
    
    CC_SAFE_DELETE(pBlock);
    return nullptr;
}

WallBlock::WallBlock()
{
    m_eType = MapBlock::Type::WALL;
}

WallBlock *
WallBlock::create(const std::string& filename)
{
    WallBlock * pBlock = new WallBlock();
    
    if(pBlock->initWithFile(filename))
    {
        pBlock->autorelease();
        return pBlock;
    }
    
    CC_SAFE_DELETE(pBlock);
    return nullptr;
}

BorderBlock::BorderBlock()
{
    m_eType = MapBlock::Type::BORDER;
}

BorderBlock *
BorderBlock::create(const std::string& filename)
{
    BorderBlock * pBlock = new BorderBlock();
    
    if(pBlock->initWithFile(filename))
    {
        pBlock->autorelease();
        return pBlock;
    }
    
    CC_SAFE_DELETE(pBlock);
    return nullptr;
}
