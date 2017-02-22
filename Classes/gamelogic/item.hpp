//
//  item.hpp
//  labyrinth
//
//  Created by Aleksandr Borzikh on 17.01.17.
//
//

#ifndef item_hpp
#define item_hpp

#include <array>
#include "cocos2d.h"
#include "gameobject.hpp"
#include "globals.h"

class Item : public GameObject
{
public:
    enum Type : unsigned char
    {
        KEY = 0x00,
        SWORD = 0x01
    };
public:
    Item();
    
    Item::Type      GetType() const;
    uint16_t        GetUID() const;
    void            SetUID(uint16_t);
    
    uint32_t        GetCarrierID() const;
    void            SetCarrierID(uint32_t);
    
        // animations
    virtual void    AnimationSpawn() {} ;
    virtual void    AnimationDisappear() {};
protected:
    Item::Type      m_eType;
    uint16_t        m_nUID;
    uint32_t        m_nCarrierID;
};

class Key : public Item
{
public:
    Key();
    
    virtual void    AnimationSpawn() override;
    virtual void    AnimationDisappear() override;
};

class Sword : public Item
{
public:
    Sword();
    
    void    SetAttack(int);
    int     GetAttack() const;
    
    virtual void    AnimationSpawn() override;
    virtual void    AnimationDisappear() override;
protected:
    int     m_nAttack;
};

#endif /* item_hpp */
