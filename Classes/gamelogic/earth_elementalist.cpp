//
//  earth_elementalist.cpp
//  labyrinth
//
//  Created by Aleksandr Borzikh on 03.03.17.
//
//

#include "earth_elementalist.hpp"

#include "gsnet_generated.h"

EarthElementalist::EarthElementalist()
{
    m_eHero = Hero::Type::EARTH_ELEMENTALIST;
    m_nMoveSpeed = 0.4;
    m_nDamage = 8;
    
    m_nSpell1CD = 3.0;
    m_nSpell1ACD = 0.0;
}

EarthElementalist *
EarthElementalist::create(const std::string& filename)
{
    EarthElementalist * pAir = new EarthElementalist();
    
    if(pAir->initWithFile(filename))
    {
        pAir->autorelease();
        return pAir;
    }
    
    CC_SAFE_DELETE(pAir);
    return nullptr;
}

std::vector<char>
EarthElementalist::EventSpellCast1()
{
    cocos2d::Vec2 demo_block = this->GetLogicalPosition();
    if(this->GetOrientation() == Orientation::DOWN)
    {
        --demo_block.y;
    }
    else if(this->GetOrientation() == Orientation::UP)
    {
        ++demo_block.y;
    }
    else if(this->GetOrientation() == Orientation::LEFT)
    {
        --demo_block.x;
    }
    else if(this->GetOrientation() == Orientation::RIGHT)
    {
        ++demo_block.x;
    }
    
    flatbuffers::FlatBufferBuilder builder;
    auto spell1 = GameEvent::CreateCLActionSpell(builder,
                                                 this->GetUID(),
                                                 1,
                                                 GameEvent::ActionSpellTarget_TARGET_POINT,
                                                 demo_block.x,
                                                 demo_block.y);
    auto event = GameEvent::CreateEvent(builder,
                                        GameEvent::Events_CLActionSpell,
                                        spell1.Union());
    builder.Finish(event);
    
    return std::vector<char>(builder.GetBufferPointer(),
                             builder.GetBufferPointer() + builder.GetSize());
}

void
EarthElementalist::SpellCast1()
{
    m_nSpell1ACD = m_nSpell1CD;
}

void
EarthElementalist::update(float delta)
{
    Hero::UpdateCDs(delta);
}
