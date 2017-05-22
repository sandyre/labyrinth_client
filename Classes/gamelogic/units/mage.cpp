//
//  mage.cpp
//  labyrinth
//
//  Created by Aleksandr Borzikh on 03.03.17.
//
//

#include "mage.hpp"

#include "../gameworld.hpp"
#include "gsnet_generated.h"

Mage::Mage()
{
    m_eHero = Hero::Type::MAGE;
    m_nMoveSpeed = 3.0;
    m_nMHealth = m_nHealth = 50;
    m_nBaseDamage = m_nActualDamage = 18;
    m_nArmor = 2;
    m_nMResistance = 6;
    
        // spell 1 cd
    m_aSpellCDs.push_back(std::make_tuple(true, 0.0f, 30.0f));
    
        // spell 2 cd
    m_aSpellCDs.push_back(std::make_tuple(true, 0.0f, 10.0f));
    
        // initialize ATTACK sequence
    InputSequence atk_seq(7);
    atk_seq.Refresh();
    m_aCastSequences.push_back(atk_seq);
    
        // initialize spell2 sequence (freeze)
    InputSequence sp_seq(7);
    sp_seq.Refresh();
    m_aCastSequences.push_back(sp_seq);
}

Mage *
Mage::create(const std::string& filename)
{
    Mage * pMage = new Mage();
    
    if(pMage->initWithFile(filename))
    {
        pMage->autorelease();
        return pMage;
    }
    
    CC_SAFE_DELETE(pMage);
    return nullptr;
}

void
Mage::RequestSpellCast(int index)
{
        // teleport
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
        // frostbolt
    else if(index == 1)
    {
        flatbuffers::FlatBufferBuilder builder;
        auto spell1 = GameEvent::CreateMageFreeze(builder,
                                                  m_pDuelTarget->GetUID());
        auto spell = GameEvent::CreateSpell(builder,
                                            GameEvent::Spells_MageFreeze,
                                            spell1.Union());
        auto cl_spell = GameEvent::CreateCLActionSpell(builder,
                                                       this->GetUID(),
                                                       1,
                                                       spell);
        auto event = GameEvent::CreateMessage(builder,
                                              GameEvent::Events_CLActionSpell,
                                              cl_spell.Union());
        builder.Finish(event);
        
        m_poGameWorld->m_aOutEvents.emplace(builder.GetBufferPointer(),
                                            builder.GetBufferPointer() + builder.GetSize());
    }
}

void
Mage::SpellCast(const GameEvent::SVActionSpell* spell)
{
        // spell0 == teleport
    if(spell->spell_id() == 0)
    {
            // set up CD
        std::get<0>(m_aSpellCDs[spell->spell_id()]) = false;
        std::get<1>(m_aSpellCDs[spell->spell_id()]) = std::get<2>(m_aSpellCDs[spell->spell_id()]);
        
        auto spells = static_cast<const GameEvent::Spell*>(spell->spell_info());
        auto spell_info = static_cast<const GameEvent::MageTeleport*>(spells->spell());
        
        this->SetLogicalPosition(cocos2d::Vec2(spell_info->x(),
                                               spell_info->y()));
        
        auto fade_out = cocos2d::FadeOut::create(0.3);
        auto move_to = cocos2d::MoveTo::create(0.001, LOG_TO_PHYS_COORD(this->GetLogicalPosition(),
                                                                        this->getContentSize()));
        auto fade_in = cocos2d::FadeIn::create(0.3);
        
        auto seq = cocos2d::Sequence::create(fade_out,
                                             move_to,
                                             fade_in,
                                             nullptr);
        this->runAction(seq);
    }
}

void
Mage::update(float delta)
{
    Hero::update(delta);
}
