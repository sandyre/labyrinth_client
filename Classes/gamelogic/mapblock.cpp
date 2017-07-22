//
//  mapblock.cpp
//  labyrinth
//
//  Created by Aleksandr Borzikh on 21.01.17.
//
//

#include "mapblock.hpp"


MapBlock::MapBlock(GameWorld * world, uint32_t uid)
: GameObject(world, uid),
  _type(Type::ABSTRACT)
{
    _objType = GameObject::Type::MAPBLOCK;
}


NoBlock::NoBlock(GameWorld * world, uint32_t uid, const std::string& sprite)
: MapBlock(world, uid)
{
    _type = MapBlock::Type::NOBLOCK;
    _objAttributes = GameObject::Attributes::PASSABLE;

    _sprite = cocos2d::Sprite::createWithSpriteFrameName(sprite);
    assert(_sprite != nullptr);
}


WallBlock::WallBlock(GameWorld * world, uint32_t uid, const std::string& sprite)
: MapBlock(world, uid)
{
    _type = MapBlock::Type::WALL;
    _objAttributes = GameObject::Attributes::PASSABLE;

    _sprite = cocos2d::Sprite::createWithSpriteFrameName(sprite);
    assert(_sprite != nullptr);
}


BorderBlock::BorderBlock(GameWorld * world, uint32_t uid, const std::string& sprite)
: MapBlock(world, uid)
{
    _type = MapBlock::Type::BORDER;
    _objAttributes = GameObject::Attributes::PASSABLE;

    _sprite = cocos2d::Sprite::createWithSpriteFrameName(sprite);
    assert(_sprite != nullptr);
}
