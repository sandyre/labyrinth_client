//
//  item.hpp
//  labyrinth
//
//  Created by Aleksandr Borzikh on 17.01.17.
//
//

#ifndef item_hpp
#define item_hpp

#include "gameobject.hpp"

#include <cocos2d.h>

#include <array>


class Item
    : public GameObject
{
public:
    enum class Type
    {
        ABSTRACT = 0x00,
        KEY = 0x00,
        SWORD = 0x01
    };
    
    Item::Type  GetType() const
    { return _type; }

protected:
    Item(GameWorld * world, uint32_t uid);
    
    Item::Type  _type;
};


class Key
    : public Item
{
public:
    Key(GameWorld * world, uint32_t uid, const std::string& sprite);
};


class Sword
    : public Item
{
public:
    Sword(GameWorld * world, uint32_t uid, const std::string& sprite);
};

#endif /* item_hpp */
