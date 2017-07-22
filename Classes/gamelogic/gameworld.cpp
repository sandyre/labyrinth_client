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

GameWorld::GameWorld(GameSessionDescriptor& descriptor)
: m_poLocalPlayer(nullptr),
  _mapConf(descriptor.MapConf)
{
    GameMap().GenerateMap(descriptor.MapConf, this);

    _channel = NetSystem::Instance().GetChannel("gameserver");

    for(auto& player : descriptor.Players)
    {
        Hero * pHero = nullptr;
        switch(player.Hero)
        {
        case Hero::Type::WARRIOR:
        {
            pHero = GameObject::create<Warrior>(this, player.Uid, "unit_warrior.png");
            break;
        }

        case Hero::Type::MAGE:
        {
            pHero = GameObject::create<Mage>(this, player.Uid, "unit_mage.png");
            break;
        }
        default:
        {
            assert(false);
            break;
        }
        }

        pHero->SetName(player.Name);
        m_apoObjects.push_back(pHero);
        this->addChild(pHero->GetSprite(), 10);

        if(player.Uid == descriptor.LocalPlayer.Uid)
        {
            m_poLocalPlayer = pHero;
            pHero->SetIsLocalPlayer(true);
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
            for(auto object : m_apoObjects)
            {
                if(object->GetType() == GameObject::Type::UNIT)
                {
                    auto unit = static_cast<Unit*>(object);
                    if(unit->GetUnitType() == Unit::Type::HERO)
                    {
                        auto hero = static_cast<Hero*>(unit);
                        
                        if(hero->GetUID() == gs_spawn->player_uid())
                        {
                            hero->Spawn(cocos2d::Vec2(gs_spawn->x(),
                                                      gs_spawn->y()));
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
            
            auto monster = Monster::create("unit_skeleton.png");
            auto monster = GameObject::create<Monster>(this, gs_spawn->monster_uid(), "unit_skeleton.png");
            monster->Spawn(cocos2d::Vec2(gs_spawn->x(),
                                         gs_spawn->y()));
            m_apoObjects.push_back(monster);
            this->addChild(monster->GetSprite(), 10);
            
            m_pUI->m_pBattleLogs->AddLogMessage("Skeleton spawned");
            
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
                key->SetCarrierID(0);
                
                cocos2d::Vec2 log_coords(gs_spawn->x(),
                                         gs_spawn->y());
                cocos2d::Vec2 spritePos = LOG_TO_PHYS_COORD(log_coords,
                                                            key->GetSprite()->getContentSize());
                
                key->Spawn(log_coords);
                m_apoObjects.push_back(key);
                this->addChild(key->GetSprite(), 0);
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
                m_apoObjects.push_back(grave);
                this->addChild(grave->GetSprite(), 0);
                break;
            }
                
            case Construction::Type::DOOR:
            {
                auto door = GameObject::create<Door>(this, gs_spawn->constr_uid(), "construction_door.png");
                
                cocos2d::Vec2 log_coords(gs_spawn->x(),
                                         gs_spawn->y());
                
                door->Spawn();
                m_apoObjects.push_back(door);
                this->addChild(door->GetSprite(), 0);
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
            
            for(auto object : m_apoObjects)
            {
                if(object->GetType() == GameObject::Type::UNIT)
                {
                    auto unit = static_cast<Unit*>(object);
                    
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
            Item * item = nullptr;
            Hero * player = nullptr;
            
            for(auto object : m_apoObjects)
            {
                if(object->GetUID() == gs_item->item_uid())
                {
                    item = static_cast<Item*>(object);
                }
                else if(object->GetUID() == gs_item->player_uid())
                {
                    player = static_cast<Hero*>(object);
                }
            }
            
            switch(gs_item->act_type())
            {
            case GameMessage::ActionItemType_TAKE:
            {
                item->AnimationTaken();
                player->TakeItem(item);
                
                if(item->GetType() == Item::Type::KEY)
                {
                    m_pUI->m_pBattleLogs->AddLogMessage("Someone took the key");
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
                Unit * first = nullptr;
                Unit * second = nullptr;
                
                for(auto object : m_apoObjects)
                {
                    if(object->GetUID() == sv_duel->target1_uid())
                    {
                        first = static_cast<Unit*>(object);
                    }
                    else if(object->GetUID() == sv_duel->target2_uid())
                    {
                        second = static_cast<Unit*>(object);
                    }
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
            
            Unit * player = nullptr;
            Unit * killer = nullptr;
            for(auto object : m_apoObjects)
            {
                if(object->GetUID() == gs_death->player_uid())
                {
                    player = static_cast<Unit*>(object);
                }
                else if(object->GetUID() == gs_death->killer_uid())
                {
                    killer = static_cast<Unit*>(object);
                }
            }
            
            player->Die(killer);
            killer->EndDuel();
            m_pUI->m_pBattleLogs->AddLogMessage(cocos2d::StringUtils::format("%s died",
                                                                             player->GetName().c_str()));
            
            break;
        }
            
        case GameMessage::Messages_SVRespawnPlayer:
        {
            auto gs_resp = static_cast<const GameMessage::SVRespawnPlayer*>(gs_event->payload());
            
            Unit * player = nullptr;
            
            for(auto object : m_apoObjects)
            {
                if(object->GetUID() == gs_resp->player_uid())
                {
                    player = static_cast<Unit*>(object);
                }
            }
            
            player->Respawn(cocos2d::Vec2(gs_resp->x(),
                                          gs_resp->y()));
            break;
        }
            
        case GameMessage::Messages_SVGameEnd:
        {
            auto gs_go = static_cast<const GameMessage::SVGameEnd*>(gs_event->payload());
            
            Unit * winner = nullptr;
            for(auto obj : m_apoObjects)
            {
                if(obj->GetUID() == gs_go->player_uid())
                {
                    winner = dynamic_cast<Unit*>(obj);
                    break;
                }
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
            m_pUI->addChild(winner_info);
            
            this->setCascadeOpacityEnabled(true); // TODO: should be placed in init
            auto fade_out = cocos2d::FadeOut::create(5.0f);
            auto pop_scene = cocos2d::CallFunc::create([this]()
                                                       {
                                                           this->release();
                                                           cocos2d::Director::getInstance()->popScene();
                                                       });
            auto seq = cocos2d::Sequence::create(fade_out,
                                                 cocos2d::DelayTime::create(2.0f),
                                                 pop_scene,
                                                 nullptr);
            this->runAction(seq);
            
            break;
        }
            
        case GameMessage::Messages_SVActionSpell:
        {
            auto gs_spell = static_cast<const GameMessage::SVActionSpell*>(gs_event->payload());
            
            Hero * player = nullptr;
            for(auto object : m_apoObjects)
            {
                if(object->GetUID() == gs_spell->player_uid())
                {
                    player = static_cast<Hero*>(object);
                    player->SpellCast(gs_spell);
                }
            }
            
            break;
        }

        case GameMessage::Messages_SVPing:
        {
            auto uuid = builder.CreateString(GameConfiguration::Instance().GetUUID());
            auto ping = GameMessage::CreateCLPing(builder,
                                                  m_poLocalPlayer->GetUID());
            auto msg = GameMessage::CreateMessage(builder,
                                                  uuid,
                                                  GameMessage::Messages_CLPing,
                                                  ping.Union());
            builder.Finish(msg);

            m_aOutEvents.emplace(builder.GetBufferPointer(),
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
    while(!m_aOutEvents.empty())
    {
        auto& event = m_aOutEvents.back();
        _channel->PushPacket(event);
        m_aOutEvents.pop();
    }
}

Hero *
GameWorld::GetLocalPlayer()
{
    return m_poLocalPlayer;
}

void
GameWorld::update(float delta)
{
    SendOutgoingNetEvents();
    ReceiveInputNetEvents();
    for(auto object : m_apoObjects)
    {
        object->update(delta);
    }
}

void
GameWorld::SetHUD(UIGameScene * ui)
{
    m_pUI = ui;
    
    m_poLocalPlayer->SetHUD(ui);
}
