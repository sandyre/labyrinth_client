//
//  gameobject.cpp
//  labyrinth
//
//  Created by Aleksandr Borzikh on 21.01.17.
//
//

#include "gameobject.hpp"

#include "cocos2d.h"

USING_NS_CC;

GameObject::GameObject() :
m_eObjType(GameObject::Type::UNDEFINED),
m_nUID(0),
m_stLogPosition(Vec2::ZERO)
{
    
}

GameObject::~GameObject()
{
    
}

GameObject::Type
GameObject::GetObjType() const
{
    return m_eObjType;
}

uint32_t
GameObject::GetUID() const
{
    return m_nUID;
}

void
GameObject::SetUID(uint32_t val)
{
    m_nUID = val;
}

Vec2
GameObject::GetLogicalPosition() const
{
    return m_stLogPosition;
}

void
GameObject::SetLogicalPosition(Vec2 pos)
{
    m_stLogPosition = pos;
}

//GameObject::GameObject() :
//m_pSprite(nullptr)
//{
//    
//}
//
//GameObject::~GameObject()
//{
//    if(m_pSprite != nullptr)
//    {
//        m_pSprite->removeFromParentAndCleanup(true);
//    }
//    
//    m_pSprite = nullptr;
//}
//
//GameObject::Type
//GameObject::GetObjectType() const
//{
//    return m_eObjType;
//}
//
//bool
//GameObject::InitSprite(const std::string &filename)
//{
//    m_pSprite = cocos2d::Sprite::create();
//    
//    if(m_pSprite->initWithFile(filename))
//    {
//        return true;
//    }
//    else
//    {
//        delete m_pSprite;
//        m_pSprite = nullptr;
//    }
//}
//
//cocos2d::Sprite *
//GameObject::GetSprite()
//{
//    return m_pSprite;
//}
//
//void
//GameObject::SetPosition(const cocos2d::Vec2& pos)
//{
//    m_stPosition = pos;
//}
//
//const cocos2d::Vec2&
//GameObject::GetPosition() const
//{
//    return m_stPosition;
//}
