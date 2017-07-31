//
//  construction.cpp
//  labyrinth
//
//  Created by Aleksandr Borzikh on 21.01.17.
//
//

#include "construction.hpp"


Construction::Construction(GameWorld& world, uint32_t uid)
: GameObject(world, uid),
  _type(Type::ABSTRACT)
{
    _objType = GameObject::Type::CONSTRUCTION;
    _objAttributes |= GameObject::Attributes::PASSABLE;
}


Door::Door(GameWorld& world, uint32_t uid, const std::string& sprite)
: Construction(world, uid)
{
    _type = Construction::Type::DOOR;

    _sprite = cocos2d::Sprite::createWithSpriteFrameName(sprite);
    assert(_sprite);
}


Graveyard::Graveyard(GameWorld& world, uint32_t uid, const std::string& sprite)
: Construction(world, uid)
{
    _type = Construction::Type::GRAVEYARD;

    _sprite = cocos2d::Sprite::createWithSpriteFrameName(sprite);
    assert(_sprite);
}
