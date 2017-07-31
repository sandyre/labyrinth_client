//
//  item.cpp
//  labyrinth
//
//  Created by Aleksandr Borzikh on 17.01.17.
//
//

#include "item.hpp"


Item::Item(GameWorld& world, uint32_t uid)
: GameObject(world, uid),
  _type(Type::ABSTRACT)
{
    _objType = GameObject::Type::ITEM;
    _objAttributes |= GameObject::Attributes::PASSABLE;
}


Key::Key(GameWorld& world, uint32_t uid, const std::string& sprite)
: Item(world, uid)
{
    _type = Item::Type::KEY;

    _sprite = cocos2d::Sprite::createWithSpriteFrameName(sprite);
    assert(_sprite);
}


Sword::Sword(GameWorld& world, uint32_t uid, const std::string& sprite)
: Item(world, uid)
{
    _type = Item::Type::SWORD;

    _sprite = cocos2d::Sprite::createWithSpriteFrameName(sprite);
    assert(_sprite);
}
