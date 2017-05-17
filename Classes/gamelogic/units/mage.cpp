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
    m_nBaseDamage = m_nActualDamage = 15;
    m_nArmor = 0;
    
    m_nSpell1CD = 30.0;
    m_nSpell1ACD = 0.0;
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
Mage::RequestSpellCast1()
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
Mage::SpellCast1(const GameEvent::SVActionSpell* spell)
{
    m_nSpell1ACD = m_nSpell1CD;
    
    auto spells = static_cast<const GameEvent::Spell*>(spell->spell_info());
    auto spell_info = static_cast<const GameEvent::MageTeleport*>(spells->spell());
    
    this->SetLogicalPosition(cocos2d::Vec2(spell_info->x(),
                                           spell_info->y()));
    this->setPosition(LOG_TO_PHYS_COORD(this->GetLogicalPosition(),
                                        this->getContentSize()));
}

void
Mage::update(float delta)
{
    Hero::update(delta);
}
