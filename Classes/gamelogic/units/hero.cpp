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

#include "cocos2d/cocos/ui/CocosGUI.h"

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
                            break;
                        }
                    }
                }
            }
            
            if(!duel_enter)
            {
                    // FIXME: potential disaster if we change InputEvent or MoveDirection consts
                RequestMove((MoveDirection)event);
            }
            break;
        }
        case Unit::State::DUEL:
        {
            if(m_nCurrentSequence != -1)
            {
                auto& seq = m_aCastSequences[m_nCurrentSequence];
                if(seq.sequence.front() == event)
                {
                    seq.sequence.pop_front();
                    
                        // move sprites left
                    auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
                    auto ui_seq = m_pUI->m_poBattleView->m_poActionsView->m_apActions[m_nCurrentSequence];
                    for(auto symb : ui_seq->m_pSequenceSymbols)
                    {
                        auto move_left = cocos2d::MoveBy::create(0.2,
                                                                 cocos2d::Vec2(-visibleSize.width * 0.2,
                                                                               0));
                        symb->runAction(move_left);
                    }
                    
                    if(seq.sequence.empty())
                    {
                        RequestSpellCast(m_nCurrentSequence + 1);
                        
                        seq.Refresh();
                        
                            // refresh sequence
                            // remove previous and add new
                        auto& ui_seq = m_pUI->m_poBattleView->m_poActionsView->m_apActions[m_nCurrentSequence];
                        for(auto symb : ui_seq->m_pSequenceSymbols)
                        {
                            symb->removeFromParentAndCleanup(true);
                        }
                        ui_seq->m_pSequenceSymbols.clear();
                        
                        auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
                        auto current_symbol_pos = visibleSize;
                        current_symbol_pos.width *= 0.5;
                        current_symbol_pos.height *= 0.058;
                        
                        for(auto i = 0; i < seq.sequence.size(); ++i)
                        {
                            auto symbol = cocos2d::ui::ImageView::create(BattleSwipeSprites[(int)m_aCastSequences[0].sequence[i]]);
                            symbol->setPosition(current_symbol_pos);
                            symbol->setCameraMask((unsigned short)cocos2d::CameraFlag::USER1); // to make it visible on HUD!
                            ui_seq->m_pSequenceLayout->addChild(symbol);
                            ui_seq->m_pSequenceSymbols.push_back(symbol);
                            
                            current_symbol_pos.width += (visibleSize.width * 0.2);
                        }
                        
                        m_nCurrentSequence = -1;
                        ui_seq->SetHighlighted(false);
                    }
                }
                else
                {
                    seq.Refresh();
                    
                        // refresh sequence
                        // remove previous and add new
                    auto& ui_seq = m_pUI->m_poBattleView->m_poActionsView->m_apActions[m_nCurrentSequence];
                    for(auto symb : ui_seq->m_pSequenceSymbols)
                    {
                        symb->removeFromParentAndCleanup(true);
                    }
                    ui_seq->m_pSequenceSymbols.clear();
                    
                    auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
                    auto current_symbol_pos = visibleSize;
                    current_symbol_pos.width *= 0.5;
                    current_symbol_pos.height *= 0.058;
                    
                    for(auto i = 0; i < seq.sequence.size(); ++i)
                    {
                        auto symbol = cocos2d::ui::ImageView::create(BattleSwipeSprites[(int)m_aCastSequences[0].sequence[i]]);
                        symbol->setPosition(current_symbol_pos);
                        symbol->setCameraMask((unsigned short)cocos2d::CameraFlag::USER1); // to make it visible on HUD!
                        ui_seq->m_pSequenceLayout->addChild(symbol);
                        ui_seq->m_pSequenceSymbols.push_back(symbol);
                        
                        current_symbol_pos.width += (visibleSize.width * 0.2);
                    }
                    
                    m_nCurrentSequence = -1;
                    ui_seq->SetHighlighted(false);
                }
            }
            else
            {
                for(int i = 0; i < m_aCastSequences.size(); ++i)
                {
                    auto& seq = m_aCastSequences[i];
                    if(seq.sequence.front() == event)
                    {
                        m_nCurrentSequence = i;
                        m_pUI->m_poBattleView->m_poActionsView->m_apActions[m_nCurrentSequence]->SetHighlighted(true);
                        seq.sequence.pop_front();
                        
                            // move sprites left
                        auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
                        auto ui_seq = m_pUI->m_poBattleView->m_poActionsView->m_apActions[m_nCurrentSequence];
                        for(auto symb : ui_seq->m_pSequenceSymbols)
                        {
                            auto move_left = cocos2d::MoveBy::create(0.2,
                                                                     cocos2d::Vec2(-visibleSize.width * 0.2,
                                                                                   0));
                            symb->runAction(move_left);
                        }
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
        
            // set up sequences
        auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
        for(auto seq = 0; seq < m_aCastSequences.size(); ++seq)
        {
            auto& act = bv->m_poActionsView->m_apActions[seq];
            for(auto& symb : act->m_pSequenceSymbols)
            {
                symb->removeFromParentAndCleanup(true);
            }
            act->m_pSequenceSymbols.clear();
            act->SetHighlighted(false);
            
            auto current_symbol_pos = visibleSize;
            current_symbol_pos.width *= 0.5;
            current_symbol_pos.height *= 0.058;
            for(auto i = 0; i < m_aCastSequences[seq].sequence.size(); ++i)
            {
                auto symbol = cocos2d::ui::ImageView::create(BattleSwipeSprites[(int)m_aCastSequences[seq].sequence[i]]);
                symbol->setPosition(current_symbol_pos);
                symbol->setCameraMask((unsigned short)cocos2d::CameraFlag::USER1); // to make it visible on HUD!
                act->m_pSequenceLayout->addChild(symbol);
                act->m_pSequenceSymbols.push_back(symbol);
                
                current_symbol_pos.width += (visibleSize.width * 0.2);
            }
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
