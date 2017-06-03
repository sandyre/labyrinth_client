//
//  item.cpp
//  labyrinth
//
//  Created by Aleksandr Borzikh on 17.01.17.
//
//

#include "item.hpp"

Item::Item() :
m_nCarrierID(0)
{
    m_eObjType = GameObject::Type::ITEM;
    m_nObjAttributes |= GameObject::Attributes::PASSABLE;
}

Item::Type
Item::GetType() const
{
    return m_eType;
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

Key::Key()
{
    m_eType = Item::Type::KEY;
}

Key *
Key::create(const std::string& filename)
{
    Key * pKey = new Key();
    
    if(pKey->initWithSpriteFrameName(filename))
    {
        pKey->autorelease();
        return pKey;
    }
    
    CC_SAFE_DELETE(pKey);
    return nullptr;
}

void
Key::AnimationSpawn()
{
    auto fadeIn = cocos2d::FadeIn::create(1);
    this->runAction(fadeIn);
}

void
Key::AnimationTaken()
{
    auto fadeOut = cocos2d::FadeOut::create(0.5);
    this->runAction(fadeOut);
}

void
Key::AnimationDropped()
{
    auto fadeIn = cocos2d::FadeIn::create(1);
    this->runAction(fadeIn);
}

Sword::Sword()
{
    m_eType = Item::Type::SWORD;
}

Sword *
Sword::create(const std::string& filename)
{
    Sword * pSword = new Sword();
    
    if(pSword->initWithSpriteFrameName(filename))
    {
        pSword->autorelease();
        return pSword;
    }
    
    CC_SAFE_DELETE(pSword);
    return nullptr;
}

void
Sword::AnimationSpawn()
{
    auto fadeIn = cocos2d::FadeIn::create(1);
    this->runAction(fadeIn);
}

void
Sword::AnimationTaken()
{
    auto fadeOut = cocos2d::FadeOut::create(0.5);
    this->runAction(fadeOut);
}

void
Sword::AnimationDropped()
{
    auto fadeIn = cocos2d::FadeIn::create(0.5);
    this->runAction(fadeIn);
}
