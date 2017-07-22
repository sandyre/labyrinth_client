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
#include "../../gsnet_generated.h"

//Rogue::Rogue()
//{
//    m_eHero = Hero::Type::ROGUE;
//    m_nMoveSpeed = 8.0;
//    m_nMHealth = m_nHealth = 50;
//    m_nBaseDamage = m_nActualDamage = 8;
//    m_nArmor = 2;
//    
//        // spell 1 cd
//    m_aSpellCDs.push_back(std::make_tuple(true, 0.0f, 30.0f));
//    
//        // spell 2 cd
//    m_aSpellCDs.push_back(std::make_tuple(true, 0.0f, 15.0f));
//    
//        // initialize ATTACK sequence
//    InputSequence seq(5, InputEvent::SWIPE_DOWN);
//    seq.Refresh();
//    m_aCastSequences.push_back(seq);
//}
//
//Rogue *
//Rogue::create(const std::string& filename)
//{
//    Rogue * pRogue = new Rogue();
//    
//    if(pRogue->initWithFile(filename))
//    {
//        pRogue->autorelease();
//        return pRogue;
//    }
//    
//    CC_SAFE_DELETE(pRogue);
//    return nullptr;
//}
//
//void
//Rogue::RequestSpellCast(int index)
//{
//        // invisibility cast
//    if(index == 0)
//    {
//        flatbuffers::FlatBufferBuilder builder;
//        auto spell1 = GameMessage::CreateCLActionSpell(builder,
//                                                     this->GetUID(),
//                                                     0);
//        auto event = GameMessage::CreateMessage(builder,
//                                              this->GetUID(),
//                                              GameMessage::Messages_CLActionSpell,
//                                              spell1.Union());
//        builder.Finish(event);
//        
//        m_poGameWorld->m_aOutEvents.emplace(builder.GetBufferPointer(),
//                                            builder.GetBufferPointer() + builder.GetSize());
//    }
//    else if(index == 1)
//    {
//        
//    }
//}
//
//void
//Rogue::SpellCast(const GameMessage::SVActionSpell* spell)
//{
//        // invisibility cast (0 spell)
//    if(spell->spell_id() == 0)
//    {
//            // set up CD
//        std::get<0>(m_aSpellCDs[0]) = false;
//        std::get<1>(m_aSpellCDs[0]) = std::get<2>(m_aSpellCDs[0]);
//        
//        RogueInvisibility * pInvis = new RogueInvisibility(5.0);
//        pInvis->SetTargetUnit(this);
//        this->ApplyEffect(pInvis);
//    }
//    else if(spell->spell_id() == 1)
//    {
//        
//    }
//}
//
//void
//Rogue::TakeItem(Item * item)
//{
//    Unit::TakeItem(item);
//}
//
//void
//Rogue::update(float delta)
//{
//    Hero::update(delta);
//}
