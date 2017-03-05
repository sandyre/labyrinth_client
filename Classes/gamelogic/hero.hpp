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
#include "gameobject.hpp"
#include "globals.h"
#include "item.hpp"

class Hero : public GameObject
{
public:
    enum class Type
    {
        AIR_ELEMENTALIST,
        WATER_ELEMENTALIST,
        FIRE_ELEMENTALIST,
        EARTH_ELEMENTALIST,
        UNDEFINED
    };
    struct Status
    {
        static const int MOVABLE = 0x01;
        static const int DUELABLE = 0x02;
        static const int INVULNERABLE = 0x04;
    };
    enum class State
    {
        WALKING,
        SWAMP,
        DUEL_PLAYER,
        DUEL_MONSTER,
        DEAD,
        INVISIBLE,
        INVULNERABLE
    };
    enum class Orientation
    {
        UP,
        DOWN,
        LEFT,
        RIGHT
    };
public:
    int32_t         GetStatus() const;
    
    Hero::Type      GetHero() const;
    
    Hero::State     GetState() const;
    void            SetState(Hero::State);
    
    std::string     GetNickname() const;
    void            SetNickname(const std::string&);
    
    uint32_t        GetDuelTargetID() const;
    void            SetDuelTargetID(uint32_t);
    
    uint16_t        GetHealth() const;
    void            SetHealth(uint16_t);
    
    uint16_t        GetMaxHealth() const;
    void            SetMaxHealth(uint16_t);
    
    uint16_t        GetDamage() const;
    
    std::vector<Item*>& GetInventory();
    
    virtual void    UpdateStats(); // based on inventory
    
    Hero::Orientation   GetOrientation() const;
    virtual void        SetOrientation(Hero::Orientation);
    
    virtual void    AnimationSpawn();
    virtual void    AnimationRespawn();
    virtual void    AnimationDeath();
    virtual void    AnimationTakeItem(Item*);
    virtual void    AnimationMoveTo(cocos2d::Vec2);
    virtual void    AnimationSpell1();
    
    std::vector<char>   EventMove(cocos2d::Vec2);
    std::vector<char>   EventItemTake(uint32_t /*item id*/);
    std::vector<char>   EventDuelStart(uint32_t /*enemy id*/);
    std::vector<char>   EventDuelAttack();
    
    bool                        isSpellCast1Ready() const;
    virtual std::vector<char>   EventSpellCast1() = 0;
protected:
    Hero();
    
    virtual void        UpdateCDs(float);
    virtual void        update(float);
    
    int32_t             m_nStatus;
    Hero::State         m_eState;
    Hero::Type          m_eHero;
    Hero::Orientation   m_eOrientation; // where heros eyes are looking
    uint16_t            m_nHealth;
    uint16_t            m_nMHealth;
    uint16_t            m_nDamage;
    uint32_t            m_nDTargetID;
    
    float               m_nMoveSpeed;
    float               m_nSpell1CD;
    float               m_nSpell1ACD;
    
    std::string         m_sNickname;
    std::vector<Item*>  m_aInventory;
};

#endif /* player_hpp */
