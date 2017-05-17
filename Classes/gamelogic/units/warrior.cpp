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
    m_nMHealth = m_nHealth = 50;
    m_nArmor = 6;
    m_nBaseDamage = m_nActualDamage = 10;
    
    m_nSpell1CD = 10.0;
    m_nSpell1ACD = 0.0;
    
        // initialize ATTACK sequence
    InputSequence seq(5);
    m_aCastSequences.push_back(seq);
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
Warrior::RequestSpellCast1()
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

void
Warrior::SpellCast1(const GameEvent::SVActionSpell*)
{
    m_nSpell1ACD = m_nSpell1CD;
    
    WarriorDash * pDash = new WarriorDash(3.0,
                                          5.5);
    pDash->SetTargetUnit(this);
    this->ApplyEffect(pDash);
}

void
Warrior::update(float delta)
{
    Hero::update(delta);
}
