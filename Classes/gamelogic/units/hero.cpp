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
#include "../construction.hpp"

#include <cocos/ui/CocosGUI.h>

Hero::Hero() :
m_eHero(Hero::Type::FIRST_HERO),
m_bIsLocalPlayer(false),
m_nCurrentSequence(-1)
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
    
    if(m_bIsLocalPlayer)
    {
        m_pUI->m_pHPText->setString(cocos2d::StringUtils::format("%d",
                                                                 this->GetHealth()));
        m_pUI->m_pHPBar->setPercent(((float)this->GetHealth() / this->GetMaxHealth()) * 100.0f);
        m_pUI->m_pArmor->setString(cocos2d::StringUtils::format("Phys: %d Mag: %d",
                                                                this->GetArmor(),
                                                                this->GetMagicResistance()));
        
        if(std::get<0>(m_aSpellCDs[0]) == true)
            m_pUI->m_poSkillsPanel->m_aSkillsButtons[0]->setEnabled(true);
        else
            m_pUI->m_poSkillsPanel->m_aSkillsButtons[0]->setEnabled(false);
        
        if(std::get<0>(m_aSpellCDs[1]) == true)
            m_pUI->m_poBattleView->m_poActionsView->m_apActions[0]->m_pIcon->setEnabled(true);
        else
            m_pUI->m_poBattleView->m_poActionsView->m_apActions[0]->m_pIcon->setEnabled(false);
        
        if(std::get<0>(m_aSpellCDs[2]) == true)
            m_pUI->m_poBattleView->m_poActionsView->m_apActions[1]->m_pIcon->setEnabled(true);
        else
            m_pUI->m_poBattleView->m_poActionsView->m_apActions[1]->m_pIcon->setEnabled(false);
        
    }
}

void
Hero::ApplyInputEvent(InputEvent event)
{
        // if input is disabled, nothing happens
    if(!(m_nUnitAttributes & Unit::Attributes::INPUT))
        return;
    
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
            else if(event == InputEvent::SPELL_CAST_0_CLICK)
            {
                if(std::get<0>(m_aSpellCDs[0]) == true)
                    this->RequestSpellCast(0);
                break;
            }
            
            auto next_pos = m_stLogPosition;
            if(event == InputEvent::SWIPE_DOWN)
            {
                --next_pos.y;
            }
            else if(event == InputEvent::SWIPE_UP)
            {
                ++next_pos.y;
            }
            else if(event == InputEvent::SWIPE_LEFT)
            {
                --next_pos.x;
            }
            else if(event == InputEvent::SWIPE_RIGHT)
            {
                ++next_pos.x;
            }
            
                // check that we have key
            bool has_key = false;
            for(auto& item : m_aInventory)
            {
                if(item->GetType() == Item::Type::KEY)
                {
                    has_key = true;
                    break;
                }
            }
            
                // check that we are to leave the labyrinth
            if(has_key)
            {
                for(auto obj : m_poGameWorld->m_apoObjects)
                {
                    if(obj->GetObjType() == GameObject::Type::CONSTRUCTION &&
                       static_cast<Construction*>(obj)->GetType() == Construction::Type::DOOR)
                    {
                        if(obj->GetLogicalPosition() == next_pos)
                        {
                                // we are ready to leave
                            flatbuffers::FlatBufferBuilder builder;
                            auto req_win = GameEvent::CreateCLRequestWin(builder,
                                                                         this->GetUID());
                            auto msg = GameEvent::CreateMessage(builder,
                                                                GameEvent::Events_CLRequestWin,
                                                                req_win.Union());
                            builder.Finish(msg);
                            m_poGameWorld->m_aOutEvents.emplace(builder.GetBufferPointer(),
                                                                builder.GetBufferPointer() + builder.GetSize());
                            return;
                        }
                    }
                }
            }
            
                // check that there is no opponent in path
            bool duel_enter = false;
            if(m_nUnitAttributes & Unit::Attributes::DUELABLE)
            {
                for(auto object : m_poGameWorld->m_apoObjects)
                {
                    if(object->GetUID() != this->GetUID() &&
                       object->GetObjType() == GameObject::Type::UNIT)
                    {
                        auto unit = dynamic_cast<Unit*>(object);
                        if(unit->GetState() == Unit::State::WALKING &&
                           (unit->GetUnitAttributes() & Unit::Attributes::DUELABLE) &&
                           unit->GetLogicalPosition() == next_pos)
                        {
                            RequestStartDuel(unit);
                            duel_enter = true;
                            return;
                        }
                    }
                }
            }
            
            RequestMove((MoveDirection)event);
            break;
        }
        case Unit::State::DUEL:
        {
            if(m_nCurrentSequence != -1)
            {
                auto& seq = m_aCastSequences[m_nCurrentSequence];
                auto seq_ui = m_pUI->m_poBattleView->m_poActionsView->m_apActions[m_nCurrentSequence];
                if(seq.sequence.front() == event)
                {
                        // shift sprites left
                    seq.sequence.pop_front();
                    seq_ui->ShiftLeft();
                    
                    if(seq.sequence.empty())
                    {
                        RequestSpellCast(m_nCurrentSequence + 1);
                        seq.Refresh();
                        
                            // refresh sequence
                            // remove previous and add new
                        seq_ui->Clear();
                        seq_ui->Fill(seq);
                        seq_ui->m_pIcon->setEnabled(false);
                        
                        m_nCurrentSequence = -1;
                        seq_ui->SetHighlighted(false);
                    }
                }
                else
                {
                    seq.Refresh();
                    
                        // refresh sequence
                        // remove previous and add new
                    seq_ui->Clear();
                    seq_ui->Fill(seq);
                    
                    m_nCurrentSequence = -1;
                    seq_ui->SetHighlighted(false);
                }
            }
            else
            {
                for(int i = 0; i < m_aCastSequences.size(); ++i)
                {
                    auto& seq = m_aCastSequences[i];
                    if(std::get<0>(m_aSpellCDs[i+1]) == false)
                        continue;
                    
                    auto seq_ui = m_pUI->m_poBattleView->m_poActionsView->m_apActions[i];
                    if(seq.sequence.front() == event)
                    {
                        seq.sequence.pop_front();
                        m_nCurrentSequence = i;
                        seq_ui->SetHighlighted(true);
                        
                            // move sprites left
                        seq_ui->ShiftLeft();
                        break;
                    }
                }
            }
            break;
        }
    }
}

void
Hero::StartDuel(Unit * enemy)
{
    Unit::StartDuel(enemy);
    
    if(m_bIsLocalPlayer)
    {
        auto bv = m_pUI->m_poBattleView;
        
            // set up sequence
        auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
        for(auto seq = 0; seq < m_aCastSequences.size(); ++seq)
        {
            m_aCastSequences[seq].Refresh();
            auto& act = bv->m_poActionsView->m_apActions[seq];
            act->Clear();
            act->Fill(m_aCastSequences[seq]);
            act->SetHighlighted(false);
        }
        bv->setVisible(true);
    }
}

void
Hero::EndDuel()
{
    Unit::EndDuel();
    
    if(m_bIsLocalPlayer)
    {
        m_pUI->m_poBattleView->setVisible(false);
    }
}

void
Hero::SetIsLocalPlayer(bool val)
{
    m_bIsLocalPlayer = val;
}

void
Hero::SetHUD(UIGameScene* ui)
{
    m_pUI = ui;
}
