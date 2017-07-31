//
//  monster.cpp
//  labyrinth
//
//  Created by Aleksandr Borzikh on 17.01.17.
//
//

#include "monster.hpp"

#include <cocos2d.h>


Monster::Monster(GameWorld& world, uint32_t uid, const std::string& sprite)
: Unit(world, uid)
{
    _type = Unit::Type::MONSTER;
    
    _baseDamage = 10;
    _bonusDamage = 0;
    _health = _healthLimit = 50;
    _armor = 2;
    _resistance = 2;

    _sprite = cocos2d::Sprite::createWithSpriteFrameName(sprite);
    assert(_sprite);
}


void
Monster::SpellCast(const GameMessage::SVActionSpell * spell)
{
    if(spell->spell_id() == 0)
    {
        DamageDescriptor dmg;
        dmg.Value = _baseDamage;
        dmg.Type = DamageDescriptor::DamageType::PHYSICAL;

        if(_duelTarget)
            _duelTarget->TakeDamage(dmg);
    }
}
