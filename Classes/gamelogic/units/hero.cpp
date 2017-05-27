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

#include <cocos/ui/CocosGUI.h>

Hero::Hero() :
m_eHero(Hero::Type::FIRST_HERO),
m_bIsLocalPlayer(false)
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
            
            auto prev_orientation = m_eOrientation;
            auto next_pos = m_stLogPosition;
            if(event == InputEvent::SWIPE_DOWN)
            {
                m_eOrientation = Orientation::DOWN;
                --next_pos.y;
            }
            else if(event == InputEvent::SWIPE_UP)
            {
                m_eOrientation = Orientation::UP;
                ++next_pos.y;
            }
            else if(event == InputEvent::SWIPE_LEFT)
            {
                m_eOrientation = Orientation::LEFT;
                --next_pos.x;
            }
            else if(event == InputEvent::SWIPE_RIGHT)
            {
                m_eOrientation = Orientation::RIGHT;
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
                           unit->GetLogicalPosition().distance(this->GetLogicalPosition()) <= 1.0)
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
                RequestMove(next_pos);
                break;
            }
            
                // flip player depending on new orientation
            if(prev_orientation != m_eOrientation)
            {
                if(m_eOrientation == Orientation::LEFT)
                {
                    this->setFlippedX(true);
                }
                else if(m_eOrientation == Orientation::RIGHT)
                {
                    this->setFlippedX(false);
                }
            }
            break;
        }
        case Unit::State::DUEL:
        {
            for(auto i = 0; i < m_aCastSequences.size(); ++i)
            {
                auto& seq = m_aCastSequences[i];
                if(seq.sequence.front() == event)
                {
                        // move sprites left
                    auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
                    auto attack_seq = m_pUI->m_poBattleView->m_poActionsView->m_apActions[i];
                    for(auto symb : attack_seq->m_pSequenceSymbols)
                    {
                        auto move_left = cocos2d::MoveBy::create(0.2,
                                                                 cocos2d::Vec2(-visibleSize.width * 0.2,
                                                                               0));
                        symb->runAction(move_left);
                    }
                    
                    seq.sequence.pop_front();
                    if(seq.sequence.size() == 0)
                    {
                        if(i == 0)
                        {
                            RequestSpellCast(1);
                        }
                        else if(i == 1)
                        {
                            RequestSpellCast(2);
                        }
                        
                        seq.Refresh();
                        
                            // refresh attack sequence
                            // remove previous and add new
                        for(auto symb : attack_seq->m_pSequenceSymbols)
                        {
                            symb->removeFromParentAndCleanup(true);
                        }
                        attack_seq->m_pSequenceSymbols.clear();
                        
                        auto current_symbol_pos = visibleSize;
                        current_symbol_pos.width *= 0.5;
                        current_symbol_pos.height *= 0.058;
                        
                        for(auto i = 0; i < seq.sequence.size(); ++i)
                        {
                            auto symbol = cocos2d::ui::ImageView::create(BattleSwipeSprites[(int)m_aCastSequences[0].sequence[i]]);
                            symbol->setPosition(current_symbol_pos);
                            symbol->setCameraMask((unsigned short)cocos2d::CameraFlag::USER1); // to make it visible on HUD!
                            attack_seq->m_pSequenceLayout->addChild(symbol);
                            attack_seq->m_pSequenceSymbols.push_back(symbol);
                            
                            current_symbol_pos.width += (visibleSize.width * 0.2);
                        }
                    }
                }
                else
                {
                    seq.Refresh();
                    
                        // refresh attack sequence
                        // remove previous and add new
                    auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
                    auto attack_seq = m_pUI->m_poBattleView->m_poActionsView->m_apActions[i];
                    for(auto symb : attack_seq->m_pSequenceSymbols)
                    {
                        symb->removeFromParentAndCleanup(true);
                    }
                    attack_seq->m_pSequenceSymbols.clear();
                    
                    auto current_symbol_pos = visibleSize;
                    current_symbol_pos.width *= 0.5;
                    current_symbol_pos.height *= 0.058;
                    
                    for(auto i = 0; i < seq.sequence.size(); ++i)
                    {
                        auto symbol = cocos2d::ui::ImageView::create(BattleSwipeSprites[(int)m_aCastSequences[0].sequence[i]]);
                        symbol->setPosition(current_symbol_pos);
                        symbol->setCameraMask((unsigned short)cocos2d::CameraFlag::USER1); // to make it visible on HUD!
                        attack_seq->m_pSequenceLayout->addChild(symbol);
                        attack_seq->m_pSequenceSymbols.push_back(symbol);
                        
                        current_symbol_pos.width += (visibleSize.width * 0.2);
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
            auto act = bv->m_poActionsView->m_apActions[seq];
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
