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
    static Player*  create(const std::string&);
    
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
    
    std::vector<uint32_t>& GetInventory();
protected:
    Player();
    
    Player::State       m_eState;
    uint16_t            m_nHealth;
    uint16_t            m_nMHealth;
    uint32_t            m_nDTargetID;
    
    std::string         m_sNickname;
    std::vector<uint32_t>   m_aInventory; // item IDs stored
};

//class Player : public GameObject
//{
//public:
//    enum class State : unsigned char
//    {
//        WALKING,
//        SWAMP,
//        DUEL_PLAYER,
//        DUEL_MONSTER,
//        DEAD
//    };
//public:
//    Player();
//    
//    std::vector<Item*>& GetInventory();
//    void              SetUID(uint32_t);
//    const uint32_t    GetUID() const;
//    std::string       GetNickname() const;
//    void              SetNickname(const std::string&);
//    
//    void              SetDuelTarget(uint32_t);
//    uint32_t          GetDuelTarget() const;
//    State             GetState() const;
//    void              SetState(State);
//    
//    void              SetHealth(uint16_t);
//    uint16_t          GetHealth() const;
//    
//    void              SetMaxHealth(uint16_t);
//    uint16_t          GetMaxHealth() const;
//    
//    void              AnimationRespawn();
//    void              AnimationDeath();
//    void              AnimationMoveTo(cocos2d::Vec2 pos);
//protected:
//    State             m_eState;
//    uint16_t          m_nHealth;
//    uint16_t          m_nMHealth;
//    uint32_t          m_nDTargetUID;
//    std::string       m_sNickname;
//    uint32_t           m_nUID;
//    std::vector<Item*> m_aInventory;
//};

#endif /* player_hpp */
