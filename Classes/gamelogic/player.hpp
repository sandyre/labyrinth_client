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

enum Hero
{
    ROGUE,
    PALADIN,
    UNDEFINED
};

class Player : public GameObject
{
public:
    enum class State
    {
        WALKING,
        SWAMP,
        DUEL_PLAYER,
        DUEL_MONSTER,
        DEAD
    };
public:
    Hero            GetHero() const;
    
    Player::State   GetState() const;
    void            SetState(Player::State);
    
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
    
    virtual void    AnimationSpawn();
    virtual void    AnimationRespawn();
    virtual void    AnimationDeath();
    virtual void    AnimationMoveTo(cocos2d::Vec2);
    virtual void    AnimationSpell1();
    
    std::vector<char>   EventMove(cocos2d::Vec2);
    std::vector<char>   EventItemTake(uint32_t /*item id*/);
    std::vector<char>   EventDuelStart(uint32_t /*enemy id*/);
    std::vector<char>   EventDuelAttack();
    
    bool                        isSpellCast1Ready() const;
    virtual std::vector<char>   EventSpellCast1() = 0;
protected:
    Player();
    
    virtual void        update(float);
    
    Player::State       m_eState;
    Hero                m_eHero;
    uint16_t            m_nHealth;
    uint16_t            m_nMHealth;
    uint16_t            m_nDamage;
    uint32_t            m_nDTargetID;
    
    float               m_nMoveSpeed;
    float               m_nSpell1CD;
    float               m_nSpell1ACD;
    
    std::string         m_sNickname;
    std::vector<Item*>   m_aInventory; // item IDs stored
};

class Rogue : public Player
{
public:
    static Rogue*   create(const std::string&);
    
    virtual std::vector<char>   EventSpellCast1() override;
    
protected:
    Rogue();
};

class Paladin : public Player
{
public:
    static Paladin* create(const std::string&);
    
    virtual std::vector<char>   EventSpellCast1() override;
protected:
    Paladin();
};

#endif /* player_hpp */
