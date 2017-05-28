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
    InputSequence(int size,
                  InputEvent starts_w)
    {
        len = size;
        starts_with = starts_w;
        
        sequence.push_back(starts_w);
        for(auto i = 1;
            i < size;
            ++i)
        {
            sequence.push_back(InputEvent::SWIPE_DOWN);
        }
    }
    
    void    Refresh()
    {
        sequence.clear();
        sequence.push_back(starts_with);
        for(auto i = 1;
            i < len;
            ++i)
        {
            sequence.push_back((InputEvent)cocos2d::RandomHelper::random_int((int)0, (int)InputEvent::SWIPE_RIGHT));
        }
    }
    
    bool is_selected = false;
    int len;
    InputEvent starts_with;
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
        LAST_HERO = 0x01
    };
public:
    Hero::Type      GetHero() const;
    
    void                        SetIsLocalPlayer(bool val);
    
        // sets HUD and configures it for current hero
    void                        SetHUD(UIGameScene* ui);
    virtual void                ApplyInputEvent(InputEvent) override;
    
    virtual void                StartDuel(Unit *) override;
    virtual void                EndDuel() override;
protected:
    Hero();

    virtual void        update(float) override;
protected:
    Hero::Type          m_eHero;
    
    int                        m_nCurrentSequence;
    std::vector<InputSequence> m_aCastSequences;
    
    bool    m_bIsLocalPlayer;
    
    UIGameScene * m_pUI;
};

#endif /* player_hpp */
