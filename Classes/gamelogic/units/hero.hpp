//
//  player.hpp
//  labyrinth
//
//  Created by Aleksandr Borzikh on 17.01.17.
//
//

#ifndef player_hpp
#define player_hpp

#include "globals.h"
#include "unit.hpp"
#include "../item.hpp"
#include "../../gsnet_generated.h"
#include "../../ui/ui_gamescene.hpp"

#include <cocos2d.h>


struct InputSequence
{
    InputSequence(int size, InputEvent starts_w)
    {
        len = size;
        starts_with = starts_w;
        
        sequence.push_back(starts_w);
        for(auto i = 1; i < size; ++i)
            sequence.push_back(InputEvent::SWIPE_DOWN);
    }
    
    void Refresh()
    {
        sequence.clear();
        sequence.push_back(starts_with);
        for(auto i = 1; i < len; ++i)
            sequence.push_back((InputEvent)cocos2d::RandomHelper::random_int((int)0, (int)InputEvent::SWIPE_RIGHT));
    }
    
    bool is_selected = false;
    int len;
    InputEvent starts_with;
    std::deque<InputEvent> sequence;
};


class Hero
    : public Unit
{
    using InputQueueElement = std::pair<float, InputEvent>;

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
    Hero::Type GetType() const
    { return _type; }
    
    virtual void update(float delta) override;

    void SetIsLocalPlayer(bool val)
    { _isLocal = val; }
    
        // sets HUD and configures it for current hero
    void SetHUD(UIGameScene* ui)
    { _hud = ui; }

    virtual void EnqueueInputEvent(InputEvent);
    
    virtual void StartDuel(const std::shared_ptr<Unit>& enemy) override;
    virtual void EndDuel() override;

protected:
    Hero(GameWorld * world, uint32_t uid);

protected:
    Hero::Type                      _type;
    bool                            _isLocal;
    int                             _currentSequenceIdx;
    std::vector<InputSequence>      _castSequences;
    UIGameScene *                   _hud;
    std::deque<InputQueueElement>   _inputEventsQueue;
};

#endif /* player_hpp */
