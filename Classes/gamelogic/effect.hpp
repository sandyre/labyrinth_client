//
//  effect.hpp
//  labyrinth
//
//  Created by Aleksandr Borzikh on 11.05.17.
//
//

#ifndef effect_hpp
#define effect_hpp

#include <cstdint>


class Unit;

class Effect
{
public:
    enum State
    {
        ACTIVE = 0x00,
        OVER = 0x01,
        PAUSED = 0x02
    };

public:
    Effect(Unit * targetUnit, float duration);
    virtual ~Effect()
    { }
    
    State GetState() const
    { return _state; }
    
    virtual void start() = 0;
    virtual void update(float);
    virtual void stop() = 0;

protected:
    State       _state;
    float       _duration;
    Unit *      _targetUnit;
};


class WarriorDash
    : public Effect
{
public:
    WarriorDash(Unit * targetUnit, float duration, float bonusMovespeed);
    
    virtual void start() override;
    virtual void stop() override;

protected:
    float       _bonusMovespeed;
};


class WarriorArmorUp
    : public Effect
{
public:
    WarriorArmorUp(Unit * targetUnit, float duration, int16_t bonusArmor);
    
    virtual void start() override;
    virtual void stop() override;

protected:
    int16_t     _bonusArmor;
};


class RogueInvisibility
    : public Effect
{
public:
    RogueInvisibility(Unit * targetUnit, float duration);
    
    virtual void start() override;
    virtual void stop() override;
};


class MageFreeze
    : public Effect
{
public:
    MageFreeze(Unit * targetUnit, float duration);
    
    virtual void start() override;
    virtual void stop() override;
};


class DuelInvulnerability
    : public Effect
{
public:
    DuelInvulnerability(Unit * targetUnit, float duration);
    
    virtual void start() override;
    virtual void stop() override;
};


class RespawnInvulnerability
    : public Effect
{
public:
    RespawnInvulnerability(Unit * targetUnit, float duration);
    
    virtual void start() override;
    virtual void stop() override;
};

#endif /* effect_hpp */
