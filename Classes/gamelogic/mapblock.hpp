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


class MapBlock
    : public GameObject
{
public:
    enum class Type
    {
        ABSTRACT,
        NOBLOCK,
        WALL,
        BORDER
    };

public:
    MapBlock::Type GetType() const
    { return _type; }
    
    virtual void update(float delta) override
    { }

protected:
    MapBlock(GameWorld& world, uint32_t uid);

protected:
    MapBlock::Type  _type;
};


class NoBlock
    : public MapBlock
{
public:
    NoBlock(GameWorld& world, uint32_t uid, const std::string& sprite);
};


class WallBlock
    : public MapBlock
{
public:
    WallBlock(GameWorld& world, uint32_t uid, const std::string& sprite);
};


class BorderBlock
    : public MapBlock
{
public:
    BorderBlock(GameWorld& world, uint32_t uid, const std::string& sprite);
};

#endif /* mapblock_hpp */
