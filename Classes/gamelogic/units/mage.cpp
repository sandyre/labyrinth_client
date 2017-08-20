//
//  mage.cpp
//  labyrinth
//
//  Created by Aleksandr Borzikh on 03.03.17.
//
//

#include "mage.hpp"

#include "gsnet_generated.h"
#include "../effect.hpp"
#include "../gameworld.hpp"
#include "../../gameconfig.hpp"


Mage::Mage(GameWorld& world, uint32_t uid, const std::string& sprite)
: Hero(world, uid)
{
    _type = Hero::Type::MAGE;
    _moveSpeed = 3.0f;
    _health = _healthLimit = 50;
    _baseDamage = 18;
    _bonusDamage = 0;
    _armor = 2;
    _resistance = 6;

    _sprite = cocos2d::Sprite::createWithSpriteFrameName(sprite);
    assert(_sprite);

    _actionExecutor.SetTarget(_sprite);

    // Animations init
    // Movement
	{
		auto animation = cocos2d::AnimationCache::getInstance()->getAnimation("unit_mage_move_l");
		animation->setRestoreOriginalFrame(true);
		animation->setLoops(1);

		auto movAnimation = cocos2d::Animate::create(animation);
		movAnimation->setDuration(1.0 / _moveSpeed);

		_animationStorage.Push("move_l", movAnimation);
	}
	{
		auto animation = cocos2d::AnimationCache::getInstance()->getAnimation("unit_mage_move_r");
		animation->setRestoreOriginalFrame(true);
		animation->setLoops(1);

		auto movAnimation = cocos2d::Animate::create(animation);
		movAnimation->setDuration(1.0 / _moveSpeed);

		_animationStorage.Push("move_r", movAnimation);
	}
    // Attack
    {
        auto animation = cocos2d::AnimationCache::getInstance()->getAnimation("unit_mage_attack");
        animation->setRestoreOriginalFrame(true);
        animation->setLoops(1);

        auto atkAnimation = cocos2d::Animate::create(animation);
        atkAnimation->setDuration(0.4f);
        atkAnimation->setTag(10);

        _animationStorage.Push("attack", atkAnimation);
    }
    // Block
    {
        auto animation = cocos2d::AnimationCache::getInstance()->getAnimation("unit_mage_freeze");
        animation->setRestoreOriginalFrame(true);
        animation->setLoops(1);

        auto freezeAnimation = cocos2d::Animate::create(animation);
        freezeAnimation->setDuration(0.5f);
        freezeAnimation->setTag(10);

        _animationStorage.Push("freeze", freezeAnimation);
    }
    // Death
    {
        auto animation = cocos2d::AnimationCache::getInstance()->getAnimation("unit_mage_death");
        animation->setRestoreOriginalFrame(true);
        animation->setLoops(1);

        auto deathAnimation = cocos2d::Animate::create(animation);
        deathAnimation->setDuration(0.8);

        _animationStorage.Push("death", deathAnimation);
    }

        // spell 1 cd (global)
    _cdManager.AddSpell(30.0f);
    
        // spell 2 cd (basic atk spell)
    _cdManager.AddSpell(0.0f);
    
        // spell 3 cd
    _cdManager.AddSpell(10.0f);
    
        // initialize spell 1 sequence
    InputSequence atk_seq(7, InputEvent::SWIPE_DOWN);
    atk_seq.Refresh();
    _castSequences.push_back(atk_seq);
    
        // initialize spell2 sequence (freeze)
    InputSequence sp_seq(7, InputEvent::SWIPE_UP);
    sp_seq.Refresh();
    _castSequences.push_back(sp_seq);
}


void
Mage::RequestSpellCast(int index)
{
        // teleport
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
        // attack
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
        // frostbolt
    else if(index == 2)
    {
        flatbuffers::FlatBufferBuilder builder;
        auto uuid = builder.CreateString(GameConfiguration::Instance().GetUUID());
        auto spell1 = GameMessage::CreateMageFreeze(builder,
                                                    _duelTarget->GetUID());
        auto spell = GameMessage::CreateSpell(builder,
                                              GameMessage::Spells_MageFreeze,
                                              spell1.Union());
        auto cl_spell = GameMessage::CreateCLActionSpell(builder,
                                                         this->GetUID(),
                                                         2,
                                                         spell);
        auto event = GameMessage::CreateMessage(builder,
                                                uuid,
                                                GameMessage::Messages_CLActionSpell,
                                                cl_spell.Union());
        builder.Finish(event);
        
        _world._outEvents.emplace(builder.GetBufferPointer(),
                                   builder.GetBufferPointer() + builder.GetSize());
    }
}


void
Mage::SpellCast(const GameMessage::SVActionSpell* spell)
{
        // spell0 == teleport
    if(spell->spell_id() == 0)
    {
            // set up CD
        _cdManager.Restart(0);
        
        auto spells = static_cast<const GameMessage::Spell*>(spell->spell_info());
        auto spell_info = static_cast<const GameMessage::MageTeleport*>(spells->spell());
        
        this->SetPosition(Point<>(spell_info->x(), spell_info->y()));
        
        auto fade_out = cocos2d::FadeOut::create(0.3);
        auto move_to = cocos2d::MoveTo::create(0.001, LOG_TO_PHYS_COORD(this->GetPosition(),
                                                                        _sprite->getContentSize()));
        auto fade_in = cocos2d::FadeIn::create(0.3);
        
        auto seq = cocos2d::Sequence::create(fade_out,
                                             move_to,
                                             fade_in,
                                             nullptr);
        _sprite->runAction(seq);
    }
        // spell1 == attack
    else if(spell->spell_id() == 1)
    {
            // set up CD
        _cdManager.Restart(1);

        auto spells = static_cast<const GameMessage::Spell*>(spell->spell_info());
        auto spell_info = static_cast<const GameMessage::MageAttack*>(spells->spell());
        
        Unit::DamageDescriptor dmg;
        dmg.Value = _baseDamage + _bonusDamage;
        dmg.Type = DamageDescriptor::DamageType::MAGICAL;
        dmg.DealerName = "";

        _duelTarget->TakeDamage(dmg);

		// animation
        _actionExecutor.LaunchAction(_animationStorage.Get("attack"), ActionExecutor::ActionType::ANIMATION);
    }
        // spell2 == freeze
    else if(spell->spell_id() == 2)
    {
            // set up CD
        _cdManager.Restart(2);
        
        auto spells = static_cast<const GameMessage::Spell*>(spell->spell_info());
        auto spell_info = static_cast<const GameMessage::MageFreeze*>(spells->spell());
        
        auto freeze = std::make_shared<MageFreeze>(_duelTarget, 3.0f);
        _duelTarget->ApplyEffect(freeze);

		// animation
        _actionExecutor.LaunchAction(_animationStorage.Get("freeze"), ActionExecutor::ActionType::ANIMATION);
    }
}
