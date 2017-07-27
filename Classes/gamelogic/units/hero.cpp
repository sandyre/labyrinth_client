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
#include "../../gsnet_generated.h"

#include <UI/CocosGUI.h>


Hero::Hero(GameWorld * world, uint32_t uid)
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
        
        if(std::get<0>(_spellsCDs[0]) == true)
            _hud->m_poSkillsPanel->m_aSkillsButtons[0]->setEnabled(true);
        else
            _hud->m_poSkillsPanel->m_aSkillsButtons[0]->setEnabled(false);
        
        if(std::get<0>(_spellsCDs[1]) == true)
            _hud->m_poBattleView->m_poActionsView->m_apActions[0]->m_pIcon->setEnabled(true);
        else
            _hud->m_poBattleView->m_poActionsView->m_apActions[0]->m_pIcon->setEnabled(false);
        
        if(std::get<0>(_spellsCDs[2]) == true)
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
                    // find item
                for(auto object : _world->_objects)
                {
                    if(object->GetType() == GameObject::Type::ITEM &&
                       object->GetPosition() == _pos)
                    {
                        RequestTakeItem(std::dynamic_pointer_cast<Item>(object));
                        break;
                    }
                }
                
                break;
            }
            else if(event == InputEvent::SPELL_CAST_0_CLICK)
            {
                if(std::get<0>(_spellsCDs[0]) == true)
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
            
                // check that we have key
            bool has_key = false;
            for(auto& item : _inventory)
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
                for(auto obj : _world->_objects)
                {
                    if(obj->GetType() == GameObject::Type::CONSTRUCTION &&
                       std::dynamic_pointer_cast<Construction>(obj)->GetType() == Construction::Type::DOOR)
                    {
                        if(obj->GetPosition() == next_pos)
                        {
                                // we are ready to leave
                            flatbuffers::FlatBufferBuilder builder;
                            auto req_win = GameMessage::CreateCLRequestWin(builder,
                                                                         this->GetUID());
                            auto msg = GameMessage::CreateMessage(builder,
                                                                  this->GetUID(),
                                                                  GameMessage::Messages_CLRequestWin,
                                                                  req_win.Union());
                            builder.Finish(msg);
                            _world->_outEvents.emplace(builder.GetBufferPointer(),
                                                       builder.GetBufferPointer() + builder.GetSize());
                            return;
                        }
                    }
                }
            }
            
                // check that there is no opponent in path
            bool duel_enter = false;
            if(_unitAttributes & Unit::Attributes::DUELABLE)
            {
                for(auto object : _world->_objects)
                {
                    if(object->GetUID() != this->GetUID() &&
                       object->GetType() == GameObject::Type::UNIT)
                    {
                        auto unit = std::dynamic_pointer_cast<Unit>(object);
                        if(unit->GetState() == Unit::State::WALKING &&
                          unit->GetUnitAttributes() & Unit::Attributes::DUELABLE &&
                          unit->GetPosition() == next_pos)
                        {
                            RequestStartDuel(unit);
                            duel_enter = true;
                            return;
                        }
                    }
                }
            }
            
            if(!(_sprite->getActionByTag(5)))
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
                    if(std::get<0>(_spellsCDs[i+1]) == false)
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
