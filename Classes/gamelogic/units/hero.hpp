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
#include "../../gsnet_generated.h"
#include "../../ui/ui_gamescene.hpp"

#include "unit.hpp"

struct InputSequence
{
    InputSequence(int size)
    {
        len = size;
        
        for(auto i = 0;
            i < size;
            ++i)
        {
            sequence.push_back(InputEvent::SWIPE_DOWN);
        }
    }
    
    void    Refresh()
    {
        sequence.clear();
        for(auto i = 0;
            i < len;
            ++i)
        {
            sequence.push_back((InputEvent)cocos2d::RandomHelper::random_int((int)0, (int)InputEvent::SWIPE_RIGHT));
        }
    }
    
    int len;
    std::deque<InputEvent> sequence;
};

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
    
    void                        SetIsLocalPlayer(bool val);
    void                        SetHUD(UIGameScene* ui);
    virtual void                ApplyInputEvent(InputEvent) override;
    
    virtual void                RequestSpellCast1() = 0;
    virtual void                SpellCast1(const GameEvent::SVActionSpell*) = 0;
    float                       GetSpell1ACD() const;
    bool                        isSpellCast1Ready() const;
    
    virtual void                RequestSpellCast2() = 0;
    
    virtual void                StartDuel(Unit *) override;
    virtual void                EndDuel() override;
protected:
    Hero();
    
    virtual void        UpdateCDs(float);
    virtual void        update(float) override;
protected:
    Hero::Type          m_eHero;
    
    std::vector<InputSequence> m_aCastSequences;
    
    float               m_nSpell1CD;
    float               m_nSpell1ACD;
    
    bool    m_bIsLocalPlayer;
    
    UIGameScene * m_pUI;
};

#endif /* player_hpp */
