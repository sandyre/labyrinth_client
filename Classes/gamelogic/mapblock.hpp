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
#include "globals.h"

class MapBlock : public GameObject
{
public:
    enum Type
    {
        NOBLOCK = 0x00,
        WALL    = 0x01,
        BORDER  = 0x02
    };
public:
    MapBlock();
    
    MapBlock::Type  GetType() const;
    bool            IsDiscovered() const;
    void            SetDiscovered(bool);
protected:
    MapBlock::Type  m_eType;
    bool            m_bDiscovered;
};

class WallBlock : public MapBlock
{
public:
    WallBlock();
};

class NoBlock : public MapBlock
{
public:
    NoBlock();
};

class BorderBlock : public MapBlock
{
public:
    BorderBlock();
};

#endif /* mapblock_hpp */
