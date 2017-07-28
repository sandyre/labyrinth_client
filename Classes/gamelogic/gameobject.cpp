//
//  gameobject.cpp
//  labyrinth
//
//  Created by Aleksandr Borzikh on 21.01.17.
//
//

#include "gameobject.hpp"

#include "../globals.h"

void
GameObject::Spawn(const Point<>& pos)
{
    _pos = pos;
    _sprite->setOpacity(0);
    _sprite->setPosition(LOG_TO_PHYS_COORD(pos, _sprite->getContentSize()));

    auto fadeIn = cocos2d::FadeIn::create(0.5f);
    _sprite->runAction(fadeIn);
}


void
GameObject::Destroy()
{
    auto fadeOut = cocos2d::FadeOut::create(0.5f);
    _sprite->runAction(fadeOut);
}
