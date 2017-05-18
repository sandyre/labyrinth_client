//
//  rogue.cpp
//  labyrinth
//
//  Created by Aleksandr Borzikh on 03.03.17.
//
//

#include "rogue.hpp"

#include "../effect.hpp"
#include "../gameworld.hpp"
#include "gsnet_generated.h"

Rogue::Rogue()
{
    m_eHero = Hero::Type::ROGUE;
    m_nMoveSpeed = 8.0;
    m_nMHealth = m_nHealth = 50;
    m_nBaseDamage = m_nActualDamage = 8;
    m_nArmor = 2;
    
    m_nSpell1CD = 30.0;
    m_nSpell1ACD = 0.0;
    
        // initialize ATTACK sequence
    InputSequence seq(5);
    seq.Refresh();
    m_aCastSequences.push_back(seq);
}

Rogue *
Rogue::create(const std::string& filename)
{
    Rogue * pRogue = new Rogue();
    
    if(pRogue->initWithFile(filename))
    {
        pRogue->autorelease();
        return pRogue;
    }
    
    CC_SAFE_DELETE(pRogue);
    return nullptr;
}

void
Rogue::RequestSpellCast1()
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
Rogue::SpellCast1(const GameEvent::SVActionSpell*)
{
    m_nSpell1ACD = m_nSpell1CD;
    
    RogueInvisibility * pInvis = new RogueInvisibility(5.0);
    pInvis->SetTargetUnit(this);
    this->ApplyEffect(pInvis);
}

void
Rogue::TakeItem(Item * item)
{
    Unit::TakeItem(item);
}

void
Rogue::update(float delta)
{
    Hero::update(delta);
}
