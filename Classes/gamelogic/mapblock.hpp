//
//  mapblock.hpp
//  labyrinth
//
//  Created by Aleksandr Borzikh on 21.01.17.
//
//

#ifndef mapblock_hpp
#define mapblock_hpp

#include "gameobject.hpp"
#include "../globals.h"

class MapBlock : public GameObject
{
public:
    enum class Type
    {
        NOBLOCK,
        WALL,
        BORDER
    };
    
public:
    MapBlock::Type  GetType() const;
    
protected:
    MapBlock();
    
    MapBlock::Type  m_eType;
};

class NoBlock : public MapBlock
{
public:
    static NoBlock* create(const std::string&);
    
protected:
    NoBlock();
};

class WallBlock : public MapBlock
{
public:
    static WallBlock* create(const std::string&);
    
protected:
    WallBlock();
};

class BorderBlock : public MapBlock
{
public:
    static BorderBlock* create(const std::string&);
    
protected:
    BorderBlock();
};

#endif /* mapblock_hpp */
