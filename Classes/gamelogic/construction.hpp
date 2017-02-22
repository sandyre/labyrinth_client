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
#include "globals.h"

class Construction : public GameObject
{
public:
    enum Type
    {
        DOOR = 0x00,
        GRAVEYARD = 0x01,
        SWAMP = 0x02
    };
public:
    Construction();
    
    Construction::Type  GetType() const;
protected:
    Construction::Type  m_eType;
};

class Door : public Construction
{
public:
    Door();
};

class Graveyard : public Construction
{
public:
    Graveyard();
};

class Swamp : public Construction
{
public:
    Swamp();
};

#endif /* construction_hpp */
