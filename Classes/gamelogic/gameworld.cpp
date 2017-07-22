//
//  gameworld.cpp
//  labyrinth
//
//  Created by Aleksandr Borzikh on 15.04.17.
//
//

#include "gameworld.hpp"

#include "pregamescene.hpp"

#include "construction.hpp"
#include "item.hpp"
#include "units/units_inc.hpp"
#include "../gameconfig.hpp"

#include <memory>


GameWorld::GameWorld(GameSessionDescriptor& descriptor)
: _mapConf(descriptor.MapConf),
  _view(cocos2d::Layer::create()),
  _channel(NetSystem::Instance().GetChannel("gameserver"))
{
    GameMap().GenerateMap(descriptor.MapConf, this);

    for(auto& player : descriptor.Players)
    {
        std::shared_ptr<Hero> hero;
        switch(player.Hero)
        {
        case Hero::Type::WARRIOR:
        {
            hero = GameObject::create<Warrior>(this, player.Uid, "unit_warrior.png");
            break;
        }

        case Hero::Type::MAGE:
        {
            hero = GameObject::create<Mage>(this, player.Uid, "unit_mage.png");
            break;
        }
        default:
        {
            assert(false);
            break;
        }
        }

        _objects.push_back(hero);
        _view->addChild(hero->GetSprite(), 10);

        if(player.Uid == descriptor.LocalPlayer.Uid)
        {
            _localPlayer = hero;
            hero->SetIsLocalPlayer(true);
        }
    }
}

void
GameWorld::ReceiveInputNetEvents()
{
    while(_channel->Available())
    {
        std::vector<uint8_t> packet = _channel->PopPacket();
        
        auto gs_event = GameMessage::GetMessage(packet.data());
        
        switch(gs_event->payload_type())
        {
        case GameMessage::Messages_SVSpawnPlayer:
        {
            auto gs_spawn = static_cast<const GameMessage::SVSpawnPlayer*>(gs_event->payload());
            for(auto object : _objects)
            {
                if(object->GetType() == GameObject::Type::UNIT)
                {
                    auto unit = std::dynamic_pointer_cast<Unit>(object);
                    if(unit->GetType() == Unit::Type::HERO)
                    {
                        auto hero = std::dynamic_pointer_cast<Hero>(unit);
                        
                        if(hero->GetUID() == gs_spawn->player_uid())
                        {
                            hero->Spawn(cocos2d::Vec2(gs_spawn->x(), gs_spawn->y()));
                            break;
                        }
                    }
                }
            }
            
            break;
        }
            
        case GameMessage::Messages_SVSpawnMonster:
        {
            auto gs_spawn = static_cast<const GameMessage::SVSpawnMonster*>(gs_event->payload());
            
            auto monster = GameObject::create<Monster>(this, gs_spawn->monster_uid(), "unit_skeleton.png");
            monster->Spawn(cocos2d::Vec2(gs_spawn->x(),
                                         gs_spawn->y()));
            _objects.push_back(monster);
            _view->addChild(monster->GetSprite(), 10);
            _ui->m_pBattleLogs->AddLogMessage("Skeleton spawned");
            
            break;
        }
            
        case GameMessage::Messages_SVSpawnItem:
        {
            auto gs_spawn = static_cast<const GameMessage::SVSpawnItem*>(gs_event->payload());
            
            switch((Item::Type)gs_spawn->item_type())
            {
            case Item::Type::KEY:
            {
                auto key = GameObject::create<Key>(this, gs_spawn->item_uid(), "item_key.png");
                
                cocos2d::Vec2 log_coords(gs_spawn->x(),
                                         gs_spawn->y());
                
                key->Spawn(log_coords);
                _objects.push_back(key);
                _view->addChild(key->GetSprite(), 0);
                break;
            }
                
            default:
                assert(false);
                break;
            }
            
            break;
        }
            
        case GameMessage::Messages_SVSpawnConstr:
        {
            auto gs_spawn = static_cast<const GameMessage::SVSpawnConstr*>(gs_event->payload());
            
            switch((Construction::Type)gs_spawn->constr_type())
            {
            case Construction::Type::GRAVEYARD:
            {
                auto grave = GameObject::create<Graveyard>(this, gs_spawn->constr_uid(), "construction_graveyard.png");
                
                cocos2d::Vec2 log_coords(gs_spawn->x(),
                                         gs_spawn->y());

                grave->Spawn(log_coords);
                _objects.push_back(grave);
                _view->addChild(grave->GetSprite(), 0);
                break;
            }
                
            case Construction::Type::DOOR:
            {
                auto door = GameObject::create<Door>(this, gs_spawn->constr_uid(), "construction_door.png");
                
                cocos2d::Vec2 log_coords(gs_spawn->x(),
                                         gs_spawn->y());
                
                door->Spawn(log_coords);
                _objects.push_back(door);
                _view->addChild(door->GetSprite(), 0);
                break;
            }
                
            default:
                assert(false);
                break;
            }
            
            break;
        }
            
        case GameMessage::Messages_SVActionMove:
        {
            auto gs_mov = static_cast<const GameMessage::SVActionMove*>(gs_event->payload());
            
            for(auto object : _objects)
            {
                if(object->GetType() == GameObject::Type::UNIT)
                {
                    auto unit = std::dynamic_pointer_cast<Unit>(object);
                    
                    if(unit->GetUID() == gs_mov->target_uid())
                    {
                        unit->Move(gs_mov);
                        break;
                    }
                }
            }
            break;
        }
            
        case GameMessage::Messages_SVActionItem:
        {
            auto gs_item = static_cast<const GameMessage::SVActionItem*>(gs_event->payload());
            std::shared_ptr<Item> item;
            std::shared_ptr<Unit> player;
            
            for(auto object : _objects)
            {
                if(object->GetUID() == gs_item->item_uid())
                    item = std::dynamic_pointer_cast<Item>(object);
                else if(object->GetUID() == gs_item->player_uid())
                    player = std::dynamic_pointer_cast<Unit>(object);
            }
            
            switch(gs_item->act_type())
            {
            case GameMessage::ActionItemType_TAKE:
            {
                item->Destroy();
                player->TakeItem(item);
                
                if(item->GetType() == Item::Type::KEY)
                {
                    _ui->m_pBattleLogs->AddLogMessage("Someone took the key");
                }
                
                break;
            }
                
            default:
                assert(false);
                break;
            }
            
            break;
        }
            
        case GameMessage::Messages_SVActionDuel:
        {
            auto sv_duel = static_cast<const GameMessage::SVActionDuel*>(gs_event->payload());
            
            switch(sv_duel->act_type())
            {
            case GameMessage::ActionDuelType_STARTED:
            {
                std::shared_ptr<Unit> first, second;
                
                for(auto object : _objects)
                {
                    if(object->GetUID() == sv_duel->target1_uid())
                        first = std::dynamic_pointer_cast<Unit>(object);
                    else if(object->GetUID() == sv_duel->target2_uid())
                        second = std::dynamic_pointer_cast<Unit>(object);
                }
                
                if(first->GetDuelTarget() != second &&
                   second->GetDuelTarget() != first)
                {
                    first->StartDuel(second);
                    second->StartDuel(first);
                }
                break;
            }
                
            default:
                assert(false);
                break;
            }
            
            break;
        }
            
        case GameMessage::Messages_SVActionDeath:
        {
            auto gs_death = static_cast<const GameMessage::SVActionDeath*>(gs_event->payload());
            
            std::shared_ptr<Unit> player, killer;

            for(auto object : _objects)
            {
                if(object->GetUID() == gs_death->player_uid())
                    player = std::dynamic_pointer_cast<Unit>(object);
                else if(object->GetUID() == gs_death->killer_uid())
                    killer = std::dynamic_pointer_cast<Unit>(object);
            }
            
            player->Die();
            killer->EndDuel();
            _ui->m_pBattleLogs->AddLogMessage(cocos2d::StringUtils::format("%s died",
                                                                           player->GetName().c_str()));
            break;
        }
            
        case GameMessage::Messages_SVRespawnPlayer:
        {
            auto gs_resp = static_cast<const GameMessage::SVRespawnPlayer*>(gs_event->payload());
            
            std::shared_ptr<Unit> player;
            
            for(auto object : _objects)
            {
                if(object->GetUID() == gs_resp->player_uid())
                    player = std::dynamic_pointer_cast<Unit>(object);
            }
            
            player->Respawn(cocos2d::Vec2(gs_resp->x(),
                                          gs_resp->y()));
            break;
        }
            
        case GameMessage::Messages_SVGameEnd:
        {
            auto gs_go = static_cast<const GameMessage::SVGameEnd*>(gs_event->payload());
            
            std::shared_ptr<Unit> winner;

            for(auto obj : _objects)
            {
                if(obj->GetUID() == gs_go->player_uid())
                    winner = std::dynamic_pointer_cast<Unit>(obj);
            }
            
                // game ends!
            auto winner_inf_pos = cocos2d::ui::RelativeLayoutParameter::create();
            winner_inf_pos->setAlign(cocos2d::ui::RelativeLayoutParameter::RelativeAlign::CENTER_IN_PARENT);
            
            auto winner_info = cocos2d::ui::Text::create(cocos2d::StringUtils::format("%s escaped... but others didn't",
                                                                                      winner->GetName().c_str()),
                                                         "fonts/alagard.ttf",
                                                         24);
            winner_info->setCameraMask((unsigned short)cocos2d::CameraFlag::USER1);
            winner_info->setLayoutParameter(winner_inf_pos);
            _ui->addChild(winner_info);
            
            _view->setCascadeOpacityEnabled(true); // TODO: should be placed in init
            auto fade_out = cocos2d::FadeOut::create(5.0f);
            auto pop_scene = cocos2d::CallFunc::create([this]()
                                                       {
                                                           _view->release();
                                                           cocos2d::Director::getInstance()->popScene();
                                                       });
            auto seq = cocos2d::Sequence::create(fade_out,
                                                 cocos2d::DelayTime::create(2.0f),
                                                 pop_scene,
                                                 nullptr);
            _view->runAction(seq);
            
            break;
        }
            
        case GameMessage::Messages_SVActionSpell:
        {
            auto gs_spell = static_cast<const GameMessage::SVActionSpell*>(gs_event->payload());
            
            std::shared_ptr<Hero> player;

            for(auto object : _objects)
            {
                if(object->GetUID() == gs_spell->player_uid())
                {
                    player = std::dynamic_pointer_cast<Hero>(object);
                    player->SpellCast(gs_spell);
                }
            }
            
            break;
        }

        case GameMessage::Messages_SVPing:
        {
            flatbuffers::FlatBufferBuilder builder;
            auto uuid = builder.CreateString(GameConfiguration::Instance().GetUUID());
            auto ping = GameMessage::CreateCLPing(builder,
                                                  _localPlayer->GetUID());
            auto msg = GameMessage::CreateMessage(builder,
                                                  uuid,
                                                  GameMessage::Messages_CLPing,
                                                  ping.Union());
            builder.Finish(msg);

            _outEvents.emplace(builder.GetBufferPointer(),
                               builder.GetBufferPointer() + builder.GetSize());

            builder.Clear();
            break;
        }
            
        default:
            assert(false);
            break;
        }
    }
}

void
GameWorld::SendOutgoingNetEvents()
{
    while(!_outEvents.empty())
    {
        auto& event = _outEvents.back();
        _channel->PushPacket(event);
        _outEvents.pop();
    }
}


void
GameWorld::update(float delta)
{
    SendOutgoingNetEvents();
    ReceiveInputNetEvents();
    for(auto object : _objects)
    {
        object->update(delta);
    }
}

void
GameWorld::SetHUD(UIGameScene * ui)
{
    _ui = ui;
    _localPlayer->SetHUD(ui);
}
