//
//  construction.hpp
//  labyrinth
//
//  Created by Aleksandr Borzikh on 21.01.17.
//
//

#ifndef construction_hpp
#define construction_hpp

#include "gameobject.hpp"


class Construction
    : public GameObject
{
public:
    enum class Type
    {
        ABSTRACT,
        DOOR,
        GRAVEYARD
    };

public:
    Construction::Type  GetType() const
    { return _type; }
    
protected:
    Construction(GameWorld * world, uint32_t uid);
    
    Construction::Type  _type;
};


class Door
    : public Construction
{
public:
    Door(GameWorld * world, uint32_t uid, const std::string& sprite);
};


class Graveyard
    : public Construction
{
public:
    Graveyard(GameWorld * world, uint32_t uid, const std::string& sprite);
};

#endif /* construction_hpp */
