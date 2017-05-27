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
#include "gsnet_generated.h"

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
    m_aSpellCDs.push_back(std::make_tuple(true, 0.0f, 5.0f));
    
        // initialize ATTACK sequence
    InputSequence atk_seq(5);
    atk_seq.Refresh();
    m_aCastSequences.push_back(atk_seq);
    
        // initialize spell2 sequence (armor up)
    InputSequence sp_seq(5);
    sp_seq.Refresh();
    m_aCastSequences.push_back(sp_seq);
}

Warrior *
Warrior::create(const std::string& filename)
{
    Warrior * pWar = new Warrior();
    
    if(pWar->initWithFile(filename))
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
