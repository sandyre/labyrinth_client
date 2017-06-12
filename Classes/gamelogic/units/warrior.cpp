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
#include "../../gsnet_generated.h"

#include <cocos/audio/include/AudioEngine.h>

Warrior::Warrior()
{
    m_eHero = Hero::Type::WARRIOR;
    m_nMoveSpeed = 2.5;
    m_nMHealth = m_nHealth = 80;
    m_nBaseDamage = m_nActualDamage = 10;
    m_nArmor = 6;
    m_nMResistance = 2;
    
        // spell 1 cd
    m_aSpellCDs.push_back(std::make_tuple(true, 0.0f, 10.0f));
    
        // spell 2 cd
    m_aSpellCDs.push_back(std::make_tuple(true, 0.0f, 0.0f));
    
        // spell 3 cd
    m_aSpellCDs.push_back(std::make_tuple(true, 0.0f, 10.0f));
    
        // initialize ATTACK sequence
    InputSequence atk_seq(5, InputEvent::SWIPE_DOWN);
    atk_seq.Refresh();
    m_aCastSequences.push_back(atk_seq);
    
        // initialize spell2 sequence (armor up)
    InputSequence sp_seq(5, InputEvent::SWIPE_UP);
    sp_seq.Refresh();
    m_aCastSequences.push_back(sp_seq);
}

Warrior *
Warrior::create(const std::string& filename)
{
    Warrior * pWar = new Warrior();
    
    if(pWar->initWithSpriteFrameName(filename))
    {
        pWar->autorelease();
        return pWar;
    }
    
    CC_SAFE_DELETE(pWar);
    return nullptr;
}

void
Warrior::RequestSpellCast(int index)
{
        // warrior dash
    if(index == 0)
    {
        flatbuffers::FlatBufferBuilder builder;
        auto spell1 = GameEvent::CreateCLActionSpell(builder,
                                                     this->GetUID(),
                                                     0);
        auto event = GameEvent::CreateMessage(builder,
                                              GameEvent::Events_CLActionSpell,
                                              spell1.Union());
        builder.Finish(event);
        
        m_poGameWorld->m_aOutEvents.emplace(builder.GetBufferPointer(),
                                            builder.GetBufferPointer() + builder.GetSize());
    }
        // warrior attack
    else if(index == 1)
    {
        flatbuffers::FlatBufferBuilder builder;
        auto spell1 = GameEvent::CreateCLActionSpell(builder,
                                                     this->GetUID(),
                                                     1);
        auto event = GameEvent::CreateMessage(builder,
                                              GameEvent::Events_CLActionSpell,
                                              spell1.Union());
        builder.Finish(event);
        
        m_poGameWorld->m_aOutEvents.emplace(builder.GetBufferPointer(),
                                            builder.GetBufferPointer() + builder.GetSize());
    }
        // warrior armor up
    else if(index == 2)
    {
        flatbuffers::FlatBufferBuilder builder;
        auto spell1 = GameEvent::CreateCLActionSpell(builder,
                                                     this->GetUID(),
                                                     2);
        auto event = GameEvent::CreateMessage(builder,
                                              GameEvent::Events_CLActionSpell,
                                              spell1.Union());
        builder.Finish(event);
        
        m_poGameWorld->m_aOutEvents.emplace(builder.GetBufferPointer(),
                                            builder.GetBufferPointer() + builder.GetSize());
    }
}

void
Warrior::SpellCast(const GameEvent::SVActionSpell* spell)
{
        // spell0 == dash
    if(spell->spell_id() == 0)
    {
            // set up CD
        std::get<0>(m_aSpellCDs[0]) = false;
        std::get<1>(m_aSpellCDs[0]) = std::get<2>(m_aSpellCDs[0]);
        
        WarriorDash * pDash = new WarriorDash(3.0,
                                              5.5);
        pDash->SetTargetUnit(this);
        this->ApplyEffect(pDash);
    }
        // spell1 == attack
    else if(spell->spell_id() == 1)
    {
            // set up CD
        std::get<0>(m_aSpellCDs[1]) = false;
        std::get<1>(m_aSpellCDs[1]) = std::get<2>(m_aSpellCDs[1]);
        
        m_pDuelTarget->TakeDamage(m_nActualDamage,
                                  Unit::DamageType::PHYSICAL,
                                  this);
    }
        // spell2 == armor up
    else if(spell->spell_id() == 2)
    {
            // set up CD
        std::get<0>(m_aSpellCDs[2]) = false;
        std::get<1>(m_aSpellCDs[2]) = std::get<2>(m_aSpellCDs[2]);
        
        WarriorArmorUp * pArmUp = new WarriorArmorUp(5.0,
                                                     4);
        pArmUp->SetTargetUnit(this);
        this->ApplyEffect(pArmUp);
    }
}

void
Warrior::update(float delta)
{
    Hero::update(delta);
}

void
Warrior::Move(const GameEvent::SVActionMove* mov)
{
    Orientation new_orient = (Orientation)mov->mov_dir();
    MoveDirection mov_dir = (MoveDirection)mov->mov_dir();
    cocos2d::Vec2 new_pos = m_stLogPosition;
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
    if(m_eOrientation != new_orient)
    {
        if(new_orient == Orientation::LEFT)
        {
            this->setFlippedX(true);
        }
        else if(new_orient == Orientation::RIGHT)
        {
            this->setFlippedX(false);
        }
    }
    
    m_eOrientation = new_orient;
    
    if(new_pos != cocos2d::Vec2(mov->x(),
                                mov->y()))
    {
        new_pos = cocos2d::Vec2(mov->x(),
                                mov->y());
    }
    
    m_stLogPosition = new_pos;
    
        // animation
    auto animation = cocos2d::AnimationCache::getInstance()->getAnimation("unit_warrior_move");
    animation->setRestoreOriginalFrame(true);
    animation->setLoops(1);
    auto mov_animation = cocos2d::Animate::create(animation);
    mov_animation->setDuration(1.0/m_nMoveSpeed);
    
    auto moveTo = cocos2d::MoveTo::create(1.0/m_nMoveSpeed,
                                          LOG_TO_PHYS_COORD(new_pos, this->getContentSize()));
    auto spawn = cocos2d::Spawn::create(mov_animation,
                                        moveTo,
                                        nullptr);
    spawn->setTag(5);
    this->runAction(spawn);
    
        // sound
    auto distance = m_poGameWorld->GetLocalPlayer()->GetLogicalPosition().distance(this->GetLogicalPosition());
    if(distance <= 10.0)
    {
        auto audio = cocos2d::experimental::AudioEngine::play2d("res/audio/step.mp3",
                                                                false,
                                                                1.0f / distance);
    }
}
