//
//  gameobject.cpp
//  labyrinth
//
//  Created by Aleksandr Borzikh on 21.01.17.
//
//

#include "gameobject.hpp"

#include <cocos2d.h>

USING_NS_CC;

GameObject::GameObject() :
m_eObjType(GameObject::Type::UNDEFINED),
m_nUID(0),
m_stLogPosition(Vec2::ZERO)
{
    m_nObjAttributes |= GameObject::Attributes::VISIBLE;
}

GameObject::~GameObject()
{
	//this->getTexture()->setAliasTexParameters();
}

GameObject::Type
GameObject::GetObjType() const
{
    return m_eObjType;
}

uint32_t
GameObject::GetObjAttributes() const
{
    return m_nObjAttributes;
}

void
GameObject::SetGameWorld(GameWorld * _gameworld)
{
    m_poGameWorld = _gameworld;
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
