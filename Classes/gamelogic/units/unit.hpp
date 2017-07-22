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
#include "../../gsnet_generated.h"

#include <queue>
#include <vector>


class Effect;
class WarriorDash;
class WarriorArmorUp;
class RogueInvisibility;
class MageFreeze;
class DuelInvulnerability;
class RespawnInvulnerability;

class Unit
    : public GameObject
{
public:
    using SpellCD = std::tuple<bool, float, float>;  // <ready, ActiveCD, nominalCD>

    enum class Type
    {
        ABSTRACT,
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
        DUEL,
        DEAD
    };

    struct DamageDescriptor
    {
        enum class DamageType
        {
            PHYSICAL = 0x00,
            MAGICAL = 0x01
        };

        DamageType  Type;
        uint16_t    Value;
        std::string DealerName;
    };

    struct Attributes
    {
        static const int INPUT = 0x01;
        static const int ATTACK = 0x02;
        static const int DUELABLE = 0x04;
    };

public:
    Unit::Type GetType() const
    { return _type; }

    uint32_t GetUnitAttributes() const
    { return _unitAttributes; }

    Unit::State GetState() const
    { return _state; }

    Unit::Orientation GetOrientation() const
    { return _orientation; }
    
    int16_t GetDamage() const
    { return _baseDamage + _bonusDamage; }

    int16_t GetHealth() const
    { return _health; }

    int16_t GetMaxHealth() const
    { return _healthLimit; }

    int16_t GetArmor() const
    { return _armor; }

    int16_t GetResistance() const
    { return _resistance; }
    
    const std::vector<std::shared_ptr<Item>>& GetInventory() const
    { return _inventory; }

    const std::shared_ptr<Unit> GetDuelTarget() const
    { return _duelTarget; }
    
        // Generic actions
    virtual void Spawn(const cocos2d::Vec2& pos) override;
    virtual void Destroy() override { }

        // Unit only actions
    virtual void TakeDamage(const DamageDescriptor& dmg);
    virtual void Respawn(const cocos2d::Vec2& pos);
    virtual void EndDuel();
    virtual void Die(Unit *);

    virtual void SpellCast(const GameMessage::SVActionSpell*) = 0;
    virtual void RequestSpellCast(int index) = 0;

    virtual void Move(const GameMessage::SVActionMove*);
    virtual void RequestMove(MoveDirection);

    virtual void StartDuel(const std::shared_ptr<Unit>& unit);
    virtual void RequestStartDuel(const std::shared_ptr<Unit>& unit);
    
    virtual void TakeItem(const std::shared_ptr<Item>& item);
    virtual void RequestTakeItem(const std::shared_ptr<Item>& item);

    virtual void DropItem(int32_t index);
    virtual void RequestDropItem(Item*) {}

    virtual void UseItem(Item*) {}
    virtual void RequestUseItem(Item*) {}
    
        // additional funcs
    virtual void ApplyEffect(const std::shared_ptr<Effect>& effect);
    
        // Received from scenes event listeners
    virtual void EnqueueInputEvent(InputEvent) {}

protected:
    Unit(GameWorld * world, uint32_t uid);
    
    virtual void update(float) override;
    virtual void UpdateCDs(float);

protected:
    Unit::Type          _type;
    uint32_t            _unitAttributes;
    Unit::State         _state;
    Unit::Orientation   _orientation;

    int16_t             _baseDamage;
    int16_t             _bonusDamage;
    int16_t             _armor;
    int16_t             _resistance;
    int16_t             _health;
    int16_t             _healthLimit;
    float               _moveSpeed;

    std::vector<SpellCD>    _spellsCDs;
    std::queue<InputEvent>  _inputEvents;
    std::vector<std::shared_ptr<Item>>      _inventory;
    std::vector<std::shared_ptr<Effect>>    _appliedEffects;
    
        // Duel-data
    std::shared_ptr<Unit>   _duelTarget;
    
        // Effects should have access to every field
    friend WarriorDash;
    friend WarriorArmorUp;
    friend RogueInvisibility;
    friend MageFreeze;
    friend DuelInvulnerability;
    friend RespawnInvulnerability;
};

#endif /* unit_hpp */
