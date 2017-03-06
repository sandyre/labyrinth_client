//
//  fire_elementalist.cpp
//  labyrinth
//
//  Created by Aleksandr Borzikh on 03.03.17.
//
//

#include "fire_elementalist.hpp"

#include "gsnet_generated.h"

FireElementalist::FireElementalist()
{
    m_eHero = Hero::Type::FIRE_ELEMENTALIST;
    m_nMoveSpeed = 0.1;
    m_nDamage = 10;
    
    m_nSpell1CD = 30.0;
    m_nSpell1ACD = 0.0;
}

FireElementalist *
FireElementalist::create(const std::string& filename)
{
    FireElementalist * pAir = new FireElementalist();
    
    if(pAir->initWithFile(filename))
    {
        pAir->autorelease();
        return pAir;
    }
    
    CC_SAFE_DELETE(pAir);
    return nullptr;
}

std::vector<char>
FireElementalist::EventSpellCast1()
{
    m_nSpell1ACD = m_nSpell1CD;
    
    flatbuffers::FlatBufferBuilder builder;
    auto spell1 = GameEvent::CreateCLActionSpell(builder,
                                                 this->GetUID(),
                                                 1,
                                                 GameEvent::ActionSpellTarget_TARGET_PLAYER,
                                                 this->GetUID());
    auto event = GameEvent::CreateMessage(builder,
                                          GameEvent::Events_CLActionSpell,
                                          spell1.Union());
    builder.Finish(event);
    
    return std::vector<char>(builder.GetBufferPointer(),
                             builder.GetBufferPointer() + builder.GetSize());
}
