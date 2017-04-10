//
//  player.hpp
//  labyrinth
//
//  Created by Aleksandr Borzikh on 17.01.17.
//
//

#ifndef player_hpp
#define player_hpp

#include "cocos2d.h"
#include "globals.h"
#include "item.hpp"

#include "unit.hpp"

class Hero : public Unit
{
public:
    enum Type : int
    {
        FIRST_HERO = 0x00,
        AIR_ELEMENTALIST = 0x00,
        WATER_ELEMENTALIST = 0x01,
        FIRE_ELEMENTALIST = 0x02,
        EARTH_ELEMENTALIST = 0x03,
        RANDOM  = 0x04
    };
public:
    Hero::Type      GetHero() const;
    
    virtual void    Turn(Unit::Orientation) override;
    
    std::vector<Item*>& GetInventory();
    
    virtual void    UpdateStats(); // based on inventory
 
    virtual void    TakeItem(Item*);
    virtual void    AnimationSpell1();
    
    std::vector<char>   EventMove(cocos2d::Vec2);
    std::vector<char>   EventItemTake(uint32_t /*item id*/);
    std::vector<char>   EventDuelStart(uint32_t /*enemy id*/);
    std::vector<char>   EventDuelAttack();
    
    float                       GetSpell1ACD() const;
    bool                        isSpellCast1Ready() const;
    virtual std::vector<char>   EventSpellCast1() = 0;
protected:
    Hero();
    
    virtual void        UpdateCDs(float);
    virtual void        update(float) override;
    
    Hero::Type          m_eHero;
    
    float               m_nSpell1CD;
    float               m_nSpell1ACD;
    
    std::vector<Item*>  m_aInventory;
};

#endif /* player_hpp */
