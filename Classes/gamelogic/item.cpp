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

Key *
Key::create(const std::string& filename)
{
    Key * pKey = new Key();
    
    if(pKey->initWithFile(filename))
    {
        pKey->autorelease();
        return pKey;
    }
    
    CC_SAFE_DELETE(pKey);
    return nullptr;
}

Sword *
Sword::create(const std::string& filename)
{
    Sword * pSword = new Sword();
    
    if(pSword->initWithFile(filename))
    {
        pSword->autorelease();
        return pSword;
    }
    
    CC_SAFE_DELETE(pSword);
    return nullptr;
}

///*
// * <Item class implementation>
// */
//
//Item::Item()
//{
//    m_eObjType = GameObject::Type::ITEM;
//}
//
//Item::Type
//Item::GetType() const
//{
//    return m_eType;
//}
//
//uint16_t
//Item::GetUID() const
//{
//    return m_nUID;
//}
//
//void
//Item::SetUID(uint16_t uid)
//{
//    m_nUID = uid;
//}
//
//uint32_t
//Item::GetCarrierID() const
//{
//    return m_nCarrierID;
//}
//
//void
//Item::SetCarrierID(uint32_t id)
//{
//    m_nCarrierID = id;
//}
//
///*
// * </Item class implementation>
// */
//
///*
// * <Key class implementation>
// */
//
//Key::Key()
//{
//    m_eType = Item::Type::KEY;
//}
//
//void
//Key::AnimationSpawn()
//{
//    auto fadein = cocos2d::FadeIn::create(3.0);
//    m_pSprite->runAction(fadein);
//}
//
//void
//Key::AnimationDisappear()
//{
//    auto fadeout = cocos2d::FadeOut::create(3.0);
//    m_pSprite->runAction(fadeout);
//}
//
///*
// * </Key class implementation>
// */
//
///*
// * <Sword class implementation>
// */
//
//Sword::Sword() :
//m_nAttack(1)
//{
//    m_eType = Item::Type::SWORD;
//}
//
//void
//Sword::SetAttack(int atk)
//{
//    m_nAttack = atk;
//}
//
//int
//Sword::GetAttack() const
//{
//    return m_nAttack;
//}
//
//void
//Sword::AnimationSpawn()
//{
//    auto scale1 = cocos2d::ScaleBy::create(1.5, 2);
//    auto scale2 = cocos2d::ScaleBy::create(1.5, 0.5);
//    auto seq = cocos2d::Sequence::create(scale1, scale2, nullptr);
//    m_pSprite->runAction(seq);
//}
//
//void
//Sword::AnimationDisappear()
//{
//    auto fadeout = cocos2d::FadeOut::create(3.0);
//    m_pSprite->runAction(fadeout);
//}
//
///*
// * </Sword class implementation>
// */
