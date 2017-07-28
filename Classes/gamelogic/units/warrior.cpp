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


Warrior::Warrior(GameWorld * world, uint32_t uid, const std::string& sprite)
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

        // spell 1 cd (global)
    _spellsCDs.push_back(std::make_tuple(true, 0.0f, 10.0f));
    
        // spell 2 cd (basic atk spell)
    _spellsCDs.push_back(std::make_tuple(true, 0.0f, 0.0f));
    
        // spell 3 cd
    _spellsCDs.push_back(std::make_tuple(true, 0.0f, 10.0f));
    
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
        
        _world->_outEvents.emplace(builder.GetBufferPointer(),
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
        
        _world->_outEvents.emplace(builder.GetBufferPointer(),
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
        
        _world->_outEvents.emplace(builder.GetBufferPointer(),
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
        std::get<0>(_spellsCDs[0]) = false;
        std::get<1>(_spellsCDs[0]) = std::get<2>(_spellsCDs[0]);
        
        auto dash = std::make_shared<WarriorDash>(std::static_pointer_cast<Unit>(shared_from_this()), 3.0, 5.5);
        this->ApplyEffect(dash);
    }
        // spell1 == attack
    else if(spell->spell_id() == 1)
    {
            // set up CD
        std::get<0>(_spellsCDs[1]) = false;
        std::get<1>(_spellsCDs[1]) = std::get<2>(_spellsCDs[1]);
        
        DamageDescriptor dmg;
        dmg.Value = _baseDamage;
        dmg.Type = DamageDescriptor::DamageType::PHYSICAL;

        _duelTarget->TakeDamage(dmg);
    }
        // spell2 == armor up
    else if(spell->spell_id() == 2)
    {
            // set up CD
        std::get<0>(_spellsCDs[2]) = false;
        std::get<1>(_spellsCDs[2]) = std::get<2>(_spellsCDs[2]);
        
        auto armorUp = std::make_shared<WarriorArmorUp>(std::static_pointer_cast<Unit>(shared_from_this()), 5.0f, 4);
        this->ApplyEffect(armorUp);
    }
}


void
Warrior::update(float delta)
{
    Hero::update(delta);
}


void
Warrior::Move(const GameMessage::SVActionMove* mov)
{
    Orientation new_orient = (Orientation)mov->mov_dir();
    MoveDirection mov_dir = (MoveDirection)mov->mov_dir();
    Point<> new_pos = _pos;
    if(mov_dir == MoveDirection::UP)
    {
        ++new_pos.y;
    }
    else if(mov_dir == MoveDirection::DOWN)
    {
        --new_pos.y;
    }
    else if(mov_dir == MoveDirection::LEFT)
    {
        --new_pos.x;
    }
    else if(mov_dir == MoveDirection::RIGHT)
    {
        ++new_pos.x;
    }
    
        // change orientation properly
    if(_orientation != new_orient)
    {
        if(new_orient == Orientation::LEFT)
        {
            _sprite->setFlippedX(true);
        }
        else if(new_orient == Orientation::RIGHT)
        {
            _sprite->setFlippedX(false);
        }
    }
    
    _orientation = new_orient;
    
    if(new_pos != Point<>(mov->x(), mov->y()))
    {
        new_pos = Point<>(mov->x(), mov->y());
    }
    
    _pos = new_pos;
    
        // animation
    auto animation = cocos2d::AnimationCache::getInstance()->getAnimation("unit_warrior_move");
    animation->setRestoreOriginalFrame(true);
    animation->setLoops(1);
    auto mov_animation = cocos2d::Animate::create(animation);
    mov_animation->setDuration(1.0/_moveSpeed);
    
    auto moveTo = cocos2d::MoveTo::create(1.0/_moveSpeed,
                                          LOG_TO_PHYS_COORD(new_pos, _sprite->getContentSize()));
    auto spawn = cocos2d::Spawn::create(mov_animation,
                                        moveTo,
                                        nullptr);
    spawn->setTag(5);
    _sprite->runAction(spawn);
    
        // sound
    auto distance = _world->GetLocalPlayer()->GetPosition().Distance(this->GetPosition());
    if(distance <= 10.0)
    {
        auto audio = cocos2d::experimental::AudioEngine::play2d("res/audio/step.mp3",
                                                                false,
                                                                1.0f / distance);
    }
}
