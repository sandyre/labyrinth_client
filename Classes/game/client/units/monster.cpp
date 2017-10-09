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

    _actionExecutor.SetTarget(_sprite);

    // Animations init
    // Movement
    {
        auto animation = cocos2d::AnimationCache::getInstance()->getAnimation("unit_skeleton_move");
        animation->setRestoreOriginalFrame(true);
        animation->setLoops(1);

        auto movAnimation = cocos2d::Animate::create(animation);
        movAnimation->setDuration(1.0 / _moveSpeed);

        _animationStorage.Push("move_l", movAnimation);
    }
    {
        auto animation = cocos2d::AnimationCache::getInstance()->getAnimation("unit_skeleton_move");
        animation->setRestoreOriginalFrame(true);
        animation->setLoops(1);

        auto movAnimation = cocos2d::Animate::create(animation);
        movAnimation->setDuration(1.0 / _moveSpeed);

        _animationStorage.Push("move_r", movAnimation);
    }
    // Attack
    {
        auto animation = cocos2d::AnimationCache::getInstance()->getAnimation("unit_skeleton_attack");
        animation->setRestoreOriginalFrame(true);
        animation->setLoops(1);

        auto atkAnimation = cocos2d::Animate::create(animation);
        atkAnimation->setDuration(0.4f);
        atkAnimation->setTag(10);

        _animationStorage.Push("attack", atkAnimation);
    }
    // Death
    {
        auto animation = cocos2d::AnimationCache::getInstance()->getAnimation("unit_skeleton_death");
        animation->setRestoreOriginalFrame(true);
        animation->setLoops(1);

        auto deathAnimation = cocos2d::Animate::create(animation);
        deathAnimation->setDuration(0.8);

        _animationStorage.Push("death", deathAnimation);
    }
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
        {
            _duelTarget->TakeDamage(dmg);
            _actionExecutor.LaunchAction(_animationStorage.Get("attack"), ActionExecutor::ActionType::ANIMATION);
        }
    }
}
