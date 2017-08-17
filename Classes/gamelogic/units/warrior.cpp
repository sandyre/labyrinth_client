//
//  warrior.cpp
//  labyrinth
//
//  Created by Aleksandr Borzikh on 03.03.17.
//
//

#include "warrior.hpp"

#include "../effect.hpp"
#include "../gameworld.hpp"
#include "../../gameconfig.hpp"
#include "../../gsnet_generated.h"

#include <audio/include/AudioEngine.h>


Warrior::Warrior(GameWorld& world, uint32_t uid, const std::string& sprite)
: Hero(world, uid)
{
    _type = Hero::Type::WARRIOR;
    _moveSpeed = 2.5f;
    _health = _healthLimit = 80;
    _baseDamage = 10;
    _bonusDamage = 0;
    _armor = 6;
    _resistance = 2;

    _sprite = cocos2d::Sprite::createWithSpriteFrameName(sprite);
    assert(_sprite);

    _actionExecutor.SetTarget(_sprite);

    // Animations init
    // Movement
    {
        auto animation = cocos2d::AnimationCache::getInstance()->getAnimation("unit_warrior_move");
        animation->setRestoreOriginalFrame(true);
        animation->setLoops(1);

        auto movAnimation = cocos2d::Animate::create(animation);
        movAnimation->setDuration(1.0 / _moveSpeed);

        _animationStorage.Push("move", movAnimation);
    }
    // Attack
    {
        auto animation = cocos2d::AnimationCache::getInstance()->getAnimation("unit_warrior_attack");
        animation->setRestoreOriginalFrame(true);
        animation->setLoops(1);

        auto atkAnimation = cocos2d::Animate::create(animation);
        atkAnimation->setDuration(0.4f);
        atkAnimation->setTag(10);

        _animationStorage.Push("attack", atkAnimation);
    }
    // Block
    {
        auto animation = cocos2d::AnimationCache::getInstance()->getAnimation("unit_warrior_block");
        animation->setRestoreOriginalFrame(true);
        animation->setLoops(1);

        auto blockAnimation = cocos2d::Animate::create(animation);
        blockAnimation->setDuration(0.5f);
        blockAnimation->setTag(10);

        _animationStorage.Push("block", blockAnimation);
    }
    // Death
    {
        auto animation = cocos2d::AnimationCache::getInstance()->getAnimation("unit_warrior_death");
        animation->setRestoreOriginalFrame(true);
        animation->setLoops(1);

        auto deathAnimation = cocos2d::Animate::create(animation);
        deathAnimation->setDuration(0.8);

        _animationStorage.Push("death", deathAnimation);
    }

        // spell 1 cd (global)
    _cdManager.AddSpell(10.0f);
    
        // spell 2 cd (basic atk spell)
    _cdManager.AddSpell(0.0f);
    
        // spell 3 cd
    _cdManager.AddSpell(10.0f);
    
        // initialize spell 1 sequence
    InputSequence atk_seq(5, InputEvent::SWIPE_DOWN);
    atk_seq.Refresh();
    _castSequences.push_back(atk_seq);
    
        // initialize spell2 sequence (armor up)
    InputSequence sp_seq(5, InputEvent::SWIPE_UP);
    sp_seq.Refresh();
    _castSequences.push_back(sp_seq);
}


void
Warrior::RequestSpellCast(int index)
{
        // warrior dash
    if(index == 0)
    {
        flatbuffers::FlatBufferBuilder builder;
        auto uuid = builder.CreateString(GameConfiguration::Instance().GetUUID());
        auto spell1 = GameMessage::CreateCLActionSpell(builder,
                                                       this->GetUID(),
                                                       0);
        auto event = GameMessage::CreateMessage(builder,
                                                uuid,
                                                GameMessage::Messages_CLActionSpell,
                                                spell1.Union());
        builder.Finish(event);
        
        _world._outEvents.emplace(builder.GetBufferPointer(),
                                   builder.GetBufferPointer() + builder.GetSize());
    }
        // warrior attack
    else if(index == 1)
    {
        flatbuffers::FlatBufferBuilder builder;
        auto uuid = builder.CreateString(GameConfiguration::Instance().GetUUID());
        auto spell1 = GameMessage::CreateCLActionSpell(builder,
                                                       this->GetUID(),
                                                       1);
        auto event = GameMessage::CreateMessage(builder,
                                                uuid,
                                                GameMessage::Messages_CLActionSpell,
                                                spell1.Union());
        builder.Finish(event);
        
        _world._outEvents.emplace(builder.GetBufferPointer(),
                                   builder.GetBufferPointer() + builder.GetSize());
    }
        // warrior armor up
    else if(index == 2)
    {
        flatbuffers::FlatBufferBuilder builder;
        auto uuid = builder.CreateString(GameConfiguration::Instance().GetUUID());
        auto spell1 = GameMessage::CreateCLActionSpell(builder,
                                                       this->GetUID(),
                                                       2);
        auto event = GameMessage::CreateMessage(builder,
                                                uuid,
                                                GameMessage::Messages_CLActionSpell,
                                                spell1.Union());
        builder.Finish(event);
        
        _world._outEvents.emplace(builder.GetBufferPointer(),
                                   builder.GetBufferPointer() + builder.GetSize());
    }
}


void
Warrior::SpellCast(const GameMessage::SVActionSpell* spell)
{
        // spell0 == dash
    if(spell->spell_id() == 0)
    {
            // set up CD
        _cdManager.Restart(0);
        
        auto dash = std::make_shared<WarriorDash>(std::static_pointer_cast<Unit>(shared_from_this()), 3.0, 5.5);
        this->ApplyEffect(dash);
    }
        // spell1 == attack
    else if(spell->spell_id() == 1)
    {
            // set up CD
        _cdManager.Restart(1);
        
        DamageDescriptor dmg;
        dmg.Value = _baseDamage;
        dmg.Type = DamageDescriptor::DamageType::PHYSICAL;

        _duelTarget->TakeDamage(dmg);

            // animation
        _actionExecutor.LaunchAction(_animationStorage.Get("attack"), ActionExecutor::ActionType::ANIMATION);
    }
        // spell2 == armor up
    else if(spell->spell_id() == 2)
    {
            // set up CD
        _cdManager.Restart(2);
        
        auto armorUp = std::make_shared<WarriorArmorUp>(std::static_pointer_cast<Unit>(shared_from_this()), 5.0f, 4);
        this->ApplyEffect(armorUp);

            // animation
       _actionExecutor.LaunchAction(_animationStorage.Get("block"), ActionExecutor::ActionType::ANIMATION);
    }
}
