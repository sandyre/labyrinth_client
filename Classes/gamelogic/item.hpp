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
    enum class Type
    {
        KEY,
        SWORD
    };
    
    Item::Type  GetType() const;
    
    uint32_t    GetCarrierID() const;
    void        SetCarrierID(uint32_t);
    
    virtual void AnimationSpawn() = 0;
    virtual void AnimationTaken() = 0;
    virtual void AnimationDropped() = 0;
protected:
    Item();
    
    Item::Type  m_eType;
    uint32_t    m_nCarrierID;
};

class Key : public Item
{
public:
    static Key* create(const std::string&);
    
    virtual void AnimationSpawn() override;
    virtual void AnimationTaken() override;
    virtual void AnimationDropped() override;
protected:
    Key();
};

class Sword : public Item
{
public:
    static Sword* create(const std::string&);
    
    virtual void AnimationSpawn() override;
    virtual void AnimationTaken() override;
    virtual void AnimationDropped() override;
protected:
    Sword();
};

#endif /* item_hpp */
