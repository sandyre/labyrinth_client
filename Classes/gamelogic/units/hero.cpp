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
m_nSpell1CD(0.0),
m_nSpell1ACD(0.0),
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
    UpdateCDs(delta);
    
    if(m_bIsLocalPlayer)
    {
        m_pUI->m_pHPText->setString(cocos2d::StringUtils::format("%d",
                                                                 this->GetHealth()));
        m_pUI->m_pHPBar->setPercent(((float)this->GetHealth() / this->GetMaxHealth()) * 100.0f);
        m_pUI->m_pArmor->setString(cocos2d::StringUtils::format("Armor: %d",
                                                                this->GetArmor()));
        
        if(this->isSpellCast1Ready())
            m_pUI->m_poSkillsPanel->m_aSkillsButtons[0]->setEnabled(true);
        else
            m_pUI->m_poSkillsPanel->m_aSkillsButtons[0]->setEnabled(false);
    }
}

void
Hero::ApplyInputEvent(InputEvent event)
{
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
            for(auto i = 0; i < m_aCastSequences.size(); ++i)
            {
                auto& seq = m_aCastSequences[i];
                if(seq.sequence.front() == event)
                {
                        // move sprites left
                    auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
                    auto attack_seq = m_pUI->m_poBattleView->m_poActionsView->m_apActions[0];
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
                            
                            RequestAttack();
                        }
                    }
                }
                else
                {
                    seq.Refresh();
                    
                        // refresh attack sequence
                        // remove previous and add new
                    auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
                    auto attack_seq = m_pUI->m_poBattleView->m_poActionsView->m_apActions[0];
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
        
            // set up attack sequence
        auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
        auto attack_seq = bv->m_poActionsView->m_apActions[0];
        auto current_symbol_pos = visibleSize;
        current_symbol_pos.width *= 0.5;
        current_symbol_pos.height *= 0.058;
        for(auto i = 0; i < m_aCastSequences[0].sequence.size(); ++i)
        {
            auto symbol = cocos2d::ui::ImageView::create(BattleSwipeSprites[(int)m_aCastSequences[0].sequence[i]]);
            symbol->setPosition(current_symbol_pos);
            symbol->setCameraMask((unsigned short)cocos2d::CameraFlag::USER1); // to make it visible on HUD!
            attack_seq->m_pSequenceLayout->addChild(symbol);
            attack_seq->m_pSequenceSymbols.push_back(symbol);
            
            current_symbol_pos.width += (visibleSize.width * 0.2);
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
