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
    Unit::update(delta);
    process_input_events();
    UpdateCDs(delta);
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
                
                    // check that there is no opponent in path
                bool duel_enter = false;
                if(this->GetAttributes() & GameObject::Attributes::DUELABLE)
                {
                    for(auto object : m_poGameWorld->m_apoObjects)
                    {
                        if(object->GetObjType() == GameObject::Type::UNIT &&
                           (next_pos == object->GetLogicalPosition()) &&
                           (object->GetAttributes() & GameObject::Attributes::DUELABLE) &&
                           object->GetUID() != this->GetUID())
                        {
                            RequestStartDuel(static_cast<Unit*>(object));
                            duel_enter = true;
                        }
                    }
                }
                
                if(!duel_enter)
                {
                    RequestMove(next_pos);
                }
                break;
            }
            case Unit::State::DUEL:
            {
                for(auto i = 0;
                    i < m_aCastSequences.size();
                    ++i)
                {
                    auto& seq = m_aCastSequences[i];
                    if(seq.sequence.front() == event)
                    {
                        seq.sequence.pop();
                        
                        if(seq.sequence.empty())
                        {
                            HUDMessage succ_msg;
                            succ_msg.msg_type = HUDMessage::MessageType::DUEL_SEQ_COMPLETED;
                            
                            m_aHUDMessages.push(succ_msg);
                            
                            if(i == 0)
                            {
                                this->RequestAttack();
                            }
                        }
                        else
                        {
                            HUDMessage corr_msg;
                            corr_msg.msg_type = HUDMessage::MessageType::DUEL_SEQ_CORRECT_INPUT;
                            
                            m_aHUDMessages.push(corr_msg);
                        }
                    }
                    else
                    {
                        seq.Refresh();
                    }
                }
                
                break;
            }
        }
        
        m_aInputEvents.pop();
    }
}

void
Hero::StartDuel(Unit * enemy)
{
    Unit::StartDuel(enemy);
    
    HUDMessage msg;
    msg.msg_type = HUDMessage::MessageType::DUEL_START;
    m_aHUDMessages.push(msg);
}

void
Hero::EndDuel()
{
    Unit::EndDuel();
    
    HUDMessage msg;
    msg.msg_type = HUDMessage::MessageType::DUEL_END;
    m_aHUDMessages.push(msg);
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
