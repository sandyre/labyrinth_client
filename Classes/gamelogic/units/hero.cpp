//
//  player.cpp
//  labyrinth
//
//  Created by Aleksandr Borzikh on 17.01.17.
//
//

#include "hero.hpp"

#include "../construction.hpp"
#include "../gameworld.hpp"
#include "../../gameconfig.hpp"
#include "../../gsnet_generated.h"

#include <UI/CocosGUI.h>


Hero::Hero(GameWorld& world, uint32_t uid)
: Unit(world, uid),
  _type(Type::FIRST_HERO),
  _isLocal(),
  _currentSequenceIdx(-1)
{
    Unit::_type = Unit::Type::HERO;
}


void
Hero::update(float delta)
{
    Unit::update(delta);
    
    if(_isLocal)
    {
        _hud->_heroStats->SetHP(this->GetHealth(), this->GetMaxHealth());
        _hud->_heroStats->SetArmor(this->GetArmor());
        _hud->_heroStats->SetMagicalDamage(this->GetArmor());
        _hud->_heroStats->SetPhysicalDamage(this->GetDamage());
        _hud->_heroStats->SetMagicalDamage(0);
        
        if(_cdManager.SpellReady(0))
            _hud->m_poSkillsPanel->m_aSkillsButtons[0]->setEnabled(true);
        else
            _hud->m_poSkillsPanel->m_aSkillsButtons[0]->setEnabled(false);
        
        if(_cdManager.SpellReady(1))
            _hud->m_poBattleView->m_poActionsView->m_apActions[0]->m_pIcon->setEnabled(true);
        else
            _hud->m_poBattleView->m_poActionsView->m_apActions[0]->m_pIcon->setEnabled(false);
        
        if(_cdManager.SpellReady(2))
            _hud->m_poBattleView->m_poActionsView->m_apActions[1]->m_pIcon->setEnabled(true);
        else
            _hud->m_poBattleView->m_poActionsView->m_apActions[1]->m_pIcon->setEnabled(false);
    }
    
        // if input is disabled, nothing happens
    if(!(_unitAttributes & Unit::Attributes::INPUT))
        return;

    _inputEventsQueue.SetExpirationTime((1.0/_moveSpeed) * 1.0);
    _inputEventsQueue.Update(delta);
    
    if(!_inputEventsQueue.Empty())
    {
        switch(_state)
        {
        case Unit::State::WALKING:
        {
            auto event = _inputEventsQueue.Peek();
            if(event == InputEvent::TAKE_ITEM_BUTTON_CLICK)
            {
                _inputEventsQueue.Dequeue();

                    // find item
                auto items = _world._objectsStorage.Subset<Item>();
                for(auto item : items)
                {
                    if(item->GetPosition() == _pos)
                    {
                        RequestTakeItem(item);
                        break;
                    }
                }
                
                break;
            }
            else if(event == InputEvent::SPELL_CAST_0_CLICK)
            {
                _inputEventsQueue.Dequeue();
                if(_cdManager.SpellReady(0))
                    this->RequestSpellCast(0);
                break;
            }
            
            auto next_pos = _pos;
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
            if(_unitAttributes & Unit::Attributes::DUELABLE)
            {
                auto units = _world._objectsStorage.Subset<Unit>();
                for(auto unit : units)
                {
                    if(unit->GetUID() != this->GetUID() &&
                       unit->GetState() == Unit::State::WALKING &&
                       unit->GetUnitAttributes() & Unit::Attributes::DUELABLE &&
                       unit->GetPosition() == next_pos)
                    {
                        RequestStartDuel(unit);
                        duel_enter = true;
                        return;
                    }
                }
            }

            if (!_sprite->getNumberOfRunningActionsByTag(1))
            {
                RequestMove((MoveDirection)event);
                _inputEventsQueue.Dequeue();
            }
            break;
        }
        case Unit::State::DUEL:
        {
            auto event = _inputEventsQueue.Dequeue();
            if(_currentSequenceIdx != -1)
            {
                auto& seq = _castSequences[_currentSequenceIdx];
                auto seq_ui = _hud->m_poBattleView->m_poActionsView->m_apActions[_currentSequenceIdx];
                if(seq.sequence.front() == event)
                {
                        // shift sprites left
                    seq.sequence.pop_front();
                    seq_ui->ShiftLeft();
                    
                    if(seq.sequence.empty())
                    {
                        RequestSpellCast(_currentSequenceIdx + 1);
                        seq.Refresh();
                        
                            // refresh sequence
                            // remove previous and add new
                        seq_ui->Clear();
                        seq_ui->Fill(seq);
                        seq_ui->m_pIcon->setEnabled(false);
                        
                        _currentSequenceIdx = -1;
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
                    
                    _currentSequenceIdx = -1;
                    seq_ui->SetHighlighted(false);
                }
            }
            else
            {
                for(int i = 0; i < _castSequences.size(); ++i)
                {
                    auto& seq = _castSequences[i];
                    if(_cdManager.SpellReady(i+1) == false)
                        continue;
                    
                    auto seq_ui = _hud->m_poBattleView->m_poActionsView->m_apActions[i];
                    if(seq.sequence.front() == event)
                    {
                        seq.sequence.pop_front();
                        _currentSequenceIdx = i;
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
}


void
Hero::EnqueueInputEvent(InputEvent event)
{ _inputEventsQueue.Enqueue(event); }


void
Hero::StartDuel(const std::shared_ptr<Unit>& enemy)
{
    Unit::StartDuel(enemy);
    
    if(_isLocal)
    {
        _inputEventsQueue.Clear();
        auto bv = _hud->m_poBattleView;
        
            // set up sequence
        auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
        for(auto seq = 0; seq < _castSequences.size(); ++seq)
        {
            _castSequences[seq].Refresh();
            auto& act = bv->m_poActionsView->m_apActions[seq];
            act->Clear();
            act->Fill(_castSequences[seq]);
            act->SetHighlighted(false);
        }
        bv->setVisible(true);
    }
}


void
Hero::EndDuel()
{
    Unit::EndDuel();
    
    if(_isLocal)
        _hud->m_poBattleView->setVisible(false);
}
