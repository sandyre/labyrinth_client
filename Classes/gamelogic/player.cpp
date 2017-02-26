//
//  player.cpp
//  labyrinth
//
//  Created by Aleksandr Borzikh on 17.01.17.
//
//

#include "player.hpp"

Player::Player() :
m_eState(Player::State::WALKING),
m_nHealth(0),
m_nMHealth(0),
m_nDTargetID(0)
{
    m_eObjType = GameObject::Type::PLAYER;
}

Player *
Player::create(const std::string& filename)
{
    Player * pPlayer = new Player();
    
    if(pPlayer->initWithFile(filename))
    {
        pPlayer->autorelease();
        return pPlayer;
    }
    
    CC_SAFE_DELETE(pPlayer);
    return nullptr;
}

Player::State
Player::GetState() const
{
    return m_eState;
}

void
Player::SetState(Player::State state)
{
    m_eState = state;
}

std::string
Player::GetNickname() const
{
    return m_sNickname;
}

void
Player::SetNickname(const std::string& name)
{
    m_sNickname = name;
}

uint32_t
Player::GetDuelTargetID() const
{
    return m_nDTargetID;
}

void
Player::SetDuelTargetID(uint32_t id)
{
    m_nDTargetID = id;
}

uint16_t
Player::GetHealth() const
{
    return m_nHealth;
}

void
Player::SetHealth(uint16_t hp)
{
    m_nHealth = hp;
}

uint16_t
Player::GetMaxHealth() const
{
    return m_nMHealth;
}

void
Player::SetMaxHealth(uint16_t hp)
{
    m_nMHealth = hp;
}

std::vector<uint32_t>&
Player::GetInventory()
{
    return m_aInventory;
}

//Player::Player() :
//m_eState(State::WALKING),
//m_nUID(0),
//m_nHealth(3),
//m_nMHealth(3),
//m_nDTargetUID(0)
//{
//    m_eObjType = GameObject::Type::PLAYER;
//}
//
//std::vector<Item*>&
//Player::GetInventory()
//{
//    return m_aInventory;
//}
//
//void
//Player::SetUID(uint32_t id)
//{
//    m_nUID = id;
//}
//
//const uint32_t
//Player::GetUID() const
//{
//    return m_nUID;
//}
//
//std::string
//Player::GetNickname() const
//{
//    return m_sNickname;
//}
//
//void
//Player::SetNickname(const std::string& name)
//{
//    m_sNickname = name;
//}
//
//uint16_t
//Player::GetHealth() const
//{
//    return m_nHealth;
//}
//
//void
//Player::SetHealth(uint16_t val)
//{
//    m_nHealth = val;
//}
//
//uint16_t
//Player::GetMaxHealth() const
//{
//    return m_nMHealth;
//}
//
//void
//Player::SetMaxHealth(uint16_t val)
//{
//    m_nHealth = val;
//}
//
//void
//Player::SetState(Player::State state)
//{
//    m_eState = state;
//}
//
//Player::State
//Player::GetState() const
//{
//    return m_eState;
//}
//
//uint32_t
//Player::GetDuelTarget() const
//{
//    return m_nDTargetUID;
//}
//
//void
//Player::SetDuelTarget(uint32_t uid)
//{
//    m_nDTargetUID = uid;
//}
//
//void
//Player::AnimationRespawn()
//{
//    auto resp = cocos2d::FadeIn::create(1.0);
//    
//    this->m_pSprite->runAction(resp);
//}
//
//void
//Player::AnimationDeath()
//{
//    auto fade = cocos2d::FadeOut::create(0.5);
//    auto rotate = cocos2d::RotateBy::create(1.0, 1080);
//    auto seq = cocos2d::Sequence::create(rotate, fade, nullptr);
//    this->m_pSprite->runAction(seq);
//}
//
//void
//Player::AnimationMoveTo(cocos2d::Vec2 pos)
//{
//    cocos2d::Vec2 spritePos = LOG_TO_PHYS_COORD(pos,
//                                                m_pSprite->getContentSize());
//    auto move = cocos2d::MoveTo::create(0.01, spritePos);
//    
//    m_pSprite->runAction(move);
//}
