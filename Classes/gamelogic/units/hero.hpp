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

#include "unit.hpp"

struct HUDMessage
{
    enum MessageType
    {
        DUEL_START,
        DUEL_SEQ_CORRECT_INPUT,
        DUEL_SEQ_WRONG_INPUT,
        DUEL_SEQ_COMPLETED,
        DUEL_END
    };
    
    MessageType msg_type;
    std::array<uint8_t, 64> payload;
};

struct InputSequence
{
    InputSequence(int size)
    {
        len = size;
        
        for(auto i = 0;
            i < size;
            ++i)
        {
            sequence.push(InputEvent::SWIPE_DOWN);
        }
    }
    
    void    Refresh()
    {
        sequence = std::queue<InputEvent>();
        for(auto i = 0;
            i < len;
            ++i)
        {
            sequence.push(InputEvent::SWIPE_DOWN);
        }
    }
    
    int len;
    std::queue<InputEvent> sequence;
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
    
    const std::vector<InputSequence>& GetCastSequences() const
    {
        return m_aCastSequences;
    }
    std::queue<HUDMessage>&     GetHUDMessages()
    {
        return m_aHUDMessages;
    }
    
    virtual void                RequestSpellCast1() = 0;
    virtual void                SpellCast1(const GameEvent::SVActionSpell*) = 0;
    float                       GetSpell1ACD() const;
    bool                        isSpellCast1Ready() const;
    
    virtual void                StartDuel(Unit *) override;
    virtual void                EndDuel() override;
protected:
    Hero();
    
    virtual void        UpdateCDs(float);
    virtual void        update(float) override;
    virtual void        process_input_events() override;
    
protected:
    Hero::Type          m_eHero;
    
    std::queue<HUDMessage>  m_aHUDMessages;
    std::vector<InputSequence> m_aCastSequences;
    
    float               m_nSpell1CD;
    float               m_nSpell1ACD;
};

#endif /* player_hpp */
