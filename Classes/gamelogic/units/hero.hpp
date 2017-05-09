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
#include "../item.hpp"

#include "unit.hpp"

class Hero : public Unit
{
public:
    enum Type : int
    {
        FIRST_HERO = 0x00,
        WARRIOR = 0x00,
        MAGE = 0x01,
        ROGUE = 0x02,
        PRIEST = 0x03,
        LAST_HERO = 0x03
    };
public:
    Hero::Type      GetHero() const;
    
    virtual void                RequestSpellCast1() = 0;
    virtual void                SpellCast1() = 0;
    float                       GetSpell1ACD() const;
    bool                        isSpellCast1Ready() const;
protected:
    Hero();
    
    virtual void        UpdateCDs(float);
    virtual void        update(float) override;
    virtual void        process_input_events() override;
    
    Hero::Type          m_eHero;
    
    float               m_nSpell1CD;
    float               m_nSpell1ACD;
};

#endif /* player_hpp */
