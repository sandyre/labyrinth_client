//
//  unit.hpp
//  labyrinth
//
//  Created by Aleksandr Borzikh on 07.03.17.
//
//

#ifndef unit_hpp
#define unit_hpp

#include "../gameobject.hpp"
#include "../item.hpp"
#include "../../gsnet_generated.h"

#include <string>

class Effect;
class WarriorDash;
class WarriorArmorUp;
class RogueInvisibility;
class MageFreeze;
class DuelInvulnerability;
class RespawnInvulnerability;

class Unit : public GameObject
{
public:
    enum class Type
    {
        UNDEFINED,
        MONSTER,
        HERO
    };
    enum class Orientation
    {
        UP,
        DOWN,
        LEFT,
        RIGHT
    };
    enum class MoveDirection
    {
        UP = 0x00,
        DOWN = 0x01,
        LEFT = 0x02,
        RIGHT = 0x03
    };
    enum class State
    {
        UNDEFINED,
        WALKING,
        SWAMP,
        DUEL,
        DEAD
    };
    enum class DamageType
    {
        PHYSICAL = 0x00,
        MAGICAL = 0x01
    };
    struct Attributes
    {
        static const int INPUT = 0x01;
        static const int ATTACK = 0x02;
        static const int DUELABLE = 0x04;
    };
public:
    Unit::Type          GetUnitType() const;
    Unit::State         GetState() const;
    Unit::Orientation   GetOrientation() const;
    uint32_t            GetUnitAttributes() const;
    
    std::string         GetName() const;
    void                SetName(const std::string&);
    
    int16_t             GetDamage() const;
    int16_t             GetHealth() const;
    int16_t             GetMaxHealth() const;
    int16_t             GetArmor() const;
    int16_t             GetMagicResistance() const;
    
    std::vector<Item*>& GetInventory();
    Unit * const        GetDuelTarget() const;
    
        // Only server actions
    virtual void    TakeDamage(int16_t, DamageType, Unit *);
    virtual void    Spawn(cocos2d::Vec2);
    virtual void    Respawn(cocos2d::Vec2);
    virtual void    EndDuel();
    virtual void    Die(Unit *);
    
        // Able to do
    virtual void    RequestSpellCast(int index) = 0;
    virtual void    SpellCast(const GameEvent::SVActionSpell*) = 0;
    virtual void    RequestMove(MoveDirection);
    virtual void    Move(const GameEvent::SVActionMove*);
    virtual void    RequestStartDuel(Unit*);
    virtual void    StartDuel(Unit*);
    
        // Items manip
    virtual void    RequestTakeItem(Item*);
    virtual void    TakeItem(Item*);
    virtual void    RequestDropItem(Item*) {}
    virtual void    DropItem(int32_t index);
    virtual void    RequestUseItem(Item*) {}
    virtual void    UseItem(Item*) {}
    
        // additional funcs
    virtual void    ApplyEffect(Effect*);
    
        // Received from scenes event listeners
    virtual void    ApplyInputEvent(InputEvent) {}
protected:
    Unit();
    
    virtual void        update(float);
    virtual void        UpdateCDs(float);
protected:
    Unit::Type          m_eUnitType;
    Unit::State         m_eState;
    Unit::Orientation   m_eOrientation;
    
    uint32_t            m_nUnitAttributes;
    
    std::string         m_sName;
    
    int16_t             m_nBaseDamage;
    int16_t             m_nActualDamage;
    int16_t             m_nHealth, m_nMHealth;
    
    int16_t             m_nArmor;
    int16_t             m_nMResistance;
    
    float   m_nMoveSpeed;
    
    std::vector<Item*>  m_aInventory;
    
    std::vector<std::tuple<bool, float, float>>    m_aSpellCDs; // <ready, ActiveCD, nominalCD>
    
    std::queue<InputEvent>  m_aInputEvents;
        // Duel-data
    Unit *              m_pDuelTarget;
    
    std::vector<Effect*> m_aAppliedEffects;
        // Effects should have access to every field
    friend WarriorDash;
    friend WarriorArmorUp;
    friend RogueInvisibility;
    friend MageFreeze;
    friend DuelInvulnerability;
    friend RespawnInvulnerability;
};

#endif /* unit_hpp */
