//
//  item.cpp
//  labyrinth
//
//  Created by Aleksandr Borzikh on 17.01.17.
//
//

#include "item.hpp"

/*
 * <Item class implementation>
 */

Item::Item()
{
    m_eObjType = GameObject::Type::ITEM;
}

Item::Type
Item::GetType() const
{
    return m_eType;
}

uint16_t
Item::GetUID() const
{
    return m_nUID;
}

void
Item::SetUID(uint16_t uid)
{
    m_nUID = uid;
}

uint32_t
Item::GetCarrierID() const
{
    return m_nCarrierID;
}

void
Item::SetCarrierID(uint32_t id)
{
    m_nCarrierID = id;
}

/*
 * </Item class implementation>
 */

/*
 * <Key class implementation>
 */

Key::Key()
{
    m_eType = Item::Type::KEY;
}

void
Key::AnimationSpawn()
{
    auto fadein = cocos2d::FadeIn::create(3.0);
    m_pSprite->runAction(fadein);
}

void
Key::AnimationDisappear()
{
    auto fadeout = cocos2d::FadeOut::create(3.0);
    m_pSprite->runAction(fadeout);
}

/*
 * </Key class implementation>
 */

/*
 * <Sword class implementation>
 */

Sword::Sword() :
m_nAttack(1)
{
    m_eType = Item::Type::SWORD;
}

void
Sword::SetAttack(int atk)
{
    m_nAttack = atk;
}

int
Sword::GetAttack() const
{
    return m_nAttack;
}

void
Sword::AnimationSpawn()
{
    auto scale1 = cocos2d::ScaleBy::create(1.5, 2);
    auto scale2 = cocos2d::ScaleBy::create(1.5, 0.5);
    auto seq = cocos2d::Sequence::create(scale1, scale2, nullptr);
    m_pSprite->runAction(seq);
}

void
Sword::AnimationDisappear()
{
    auto fadeout = cocos2d::FadeOut::create(3.0);
    m_pSprite->runAction(fadeout);
}

/*
 * </Sword class implementation>
 */
