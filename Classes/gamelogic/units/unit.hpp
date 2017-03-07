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

#include <string>

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
    enum class State
    {
        UNDEFINED,
        WALKING,
        SWAMP,
        DUEL,
        DEAD
    };
    struct Status
    {
        static const int MOVABLE = 0x01;
        static const int DUELABLE = 0x02;
        static const int INVULNERABLE = 0x04;
    };
public:
    Unit::Type          GetUnitType() const;
    Unit::State         GetState() const;
    Unit::Orientation   GetOrientation() const;
    uint32_t            GetStatus() const;
    
    std::string         GetName() const;
    void                SetName(const std::string&);
    
    int16_t             GetDamage() const;
    int16_t             GetHealth() const;
    int16_t             GetMaxHealth() const;
    
    Unit * const        GetDuelTarget() const;
    
    virtual void    Spawn(cocos2d::Vec2);
    virtual void    Respawn(cocos2d::Vec2);
    virtual void    Move(cocos2d::Vec2);
    
    virtual void    StartDuel(Unit*);
    virtual void    Attack();
    virtual void    TakeDamage(int16_t);
    virtual void    EndDuel();
    
    virtual void    Die();
    virtual void    Turn(Unit::Orientation) = 0;
    
protected:
    Unit();
    
    virtual void        update(float) = 0;
protected:
    Unit::Type          m_eUnitType;
    Unit::State         m_eState;
    Unit::Orientation   m_eOrientation;
    uint32_t            m_nStatus;
    
    std::string         m_sName;
    
    int16_t             m_nBaseDamage;
    int16_t             m_nActualDamage;
    int16_t             m_nHealth, m_nMHealth;
    
    float   m_nMoveSpeed;
    
        // Duel-data
    Unit *              m_pDuelTarget;
};

#endif /* unit_hpp */
