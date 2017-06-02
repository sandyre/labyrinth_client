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
    Effect();
    virtual ~Effect();
    
    State   GetState() const;
    
    void    SetTargetUnit(Unit*);
    virtual void start() = 0;
    virtual void update(float) = 0; // called each frame by unit its applied to
    virtual void stop() = 0;
protected:
    State m_eEffState;
    float m_nADuration;
    Unit * m_pTargetUnit;
};

class WarriorDash : public Effect
{
public:
    WarriorDash(float duration,
                float bonus_movespeed);
    
    virtual void start() override;
    virtual void update(float) override;
    virtual void stop() override;
protected:
    float m_nBonusMovespeed;
};

class WarriorArmorUp : public Effect
{
public:
    WarriorArmorUp(float duration,
                   int16_t bonus_armor);
    
    virtual void start() override;
    virtual void update(float) override;
    virtual void stop() override;
protected:
    int16_t m_nBonusArmor;
};

class RogueInvisibility : public Effect
{
public:
    RogueInvisibility(float duration);
    
    virtual void start() override;
    virtual void update(float) override;
    virtual void stop() override;
};

class MageFreeze : public Effect
{
public:
    MageFreeze(float duration);
    
    virtual void start() override;
    virtual void update(float) override;
    virtual void stop() override;
};

class DuelInvulnerability : public Effect
{
public:
    DuelInvulnerability(float duration);
    
    virtual void start() override;
    virtual void update(float) override;
    virtual void stop() override;
};

class RespawnInvulnerability : public Effect
{
public:
    RespawnInvulnerability(float duration);
    
    virtual void start() override;
    virtual void update(float) override;
    virtual void stop() override;
};

#endif /* effect_hpp */
