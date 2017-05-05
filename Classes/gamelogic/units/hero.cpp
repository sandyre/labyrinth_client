//
//  player.cpp
//  labyrinth
//
//  Created by Aleksandr Borzikh on 17.01.17.
//
//

#include "hero.hpp"

#include "gsnet_generated.h"
#include "../gameworld.hpp"

Hero::Hero() :
m_eHero(Hero::Type::FIRST_HERO),
m_nSpell1CD(0.0),
m_nSpell1ACD(0.0)
{
    m_eUnitType = Unit::Type::HERO;
}

Hero::Type
Hero::GetHero() const
{
    return m_eHero;
}

void
Hero::update(float delta)
{
    process_input_events();
    UpdateCDs(delta);
    
        // check that player can enter duel, and there is an opponent ^_-!
    if(this->GetState() == Unit::State::WALKING &&
       (this->GetAttributes() & GameObject::Attributes::DUELABLE))
    {
        for(auto object : m_poGameWorld->m_apoObjects)
        {
            if(object->GetObjType() == GameObject::Type::UNIT &&
               (this->GetLogicalPosition().distance(object->GetLogicalPosition()) == 1.0) &&
               (object->GetAttributes() & GameObject::Attributes::DUELABLE) &&
               object->GetUID() != this->GetUID())
            {
                RequestStartDuel(static_cast<Unit*>(object));
                break;
            }
        }
    }
}

void
Hero::process_input_events()
{
    while(!m_aInputEvents.empty())
    {
        auto event = m_aInputEvents.front();
        switch(m_eState)
        {
            case Unit::State::WALKING:
            {
                if(event == InputEvent::TAKE_ITEM_BUTTON_CLICK)
                {
                        // find item
                    for(auto object : m_poGameWorld->m_apoObjects)
                    {
                        if(object->GetObjType() == GameObject::Type::ITEM &&
                           object->GetLogicalPosition() == m_stLogPosition)
                        {
                            auto item = static_cast<Item*>(object);
                            if(item->GetCarrierID() == 0)
                            {
                                RequestTakeItem(item);
                                break;
                            }
                        }
                    }
                    
                    break;
                }
                else if(event == InputEvent::SPELL_CAST_1_CLICK)
                {
                    if(this->isSpellCast1Ready())
                        this->RequestSpellCast1();
                    break;
                }
                auto next_pos = m_stLogPosition;
                if(event == InputEvent::SWIPE_DOWN)
                    --next_pos.y;
                else if(event == InputEvent::SWIPE_UP)
                    ++next_pos.y;
                else if(event == InputEvent::SWIPE_LEFT)
                    --next_pos.x;
                else if(event == InputEvent::SWIPE_RIGHT)
                    ++next_pos.x;
                
                RequestMove(next_pos);
                break;
            }
            case Unit::State::DUEL:
            {
                if(event == InputEvent::SWIPE_UP)
                    this->RequestAttack();
                
                break;
            }
        }
        
        m_aInputEvents.pop();
    }
}

void
Hero::UpdateCDs(float delta)
{
    if(m_nSpell1ACD > 0.0)
    {
        m_nSpell1ACD -= delta;
    }
    else
    {
        m_nSpell1ACD = 0.0;
    }
}

bool
Hero::isSpellCast1Ready() const
{
    if(m_nSpell1ACD == 0.0)
        return true;
    return false;
}

float
Hero::GetSpell1ACD() const
{
    return m_nSpell1ACD;
}
