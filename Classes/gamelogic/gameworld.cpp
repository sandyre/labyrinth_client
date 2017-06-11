//
//  gameworld.cpp
//  labyrinth
//
//  Created by Aleksandr Borzikh on 15.04.17.
//
//

#include "gameworld.hpp"

#include "construction.hpp"
#include "item.hpp"
#include "units/units_inc.hpp"
#include "../netsystem.hpp"
#include "../gameconfig.hpp"

GameWorld::GameWorld() :
m_poLocalPlayer(nullptr)
{
    
}

void
GameWorld::CreateGameMap(const GameMap::Configuration& _conf)
{
    m_stMapConf = _conf;
    
    GameMap gamemap;
    gamemap.GenerateMap(m_stMapConf, this);
}

void
GameWorld::AddPlayer(PlayerInfo player)
{
    Hero * pHero = nullptr;
    switch(player.nHeroIndex)
    {
        case Hero::Type::WARRIOR:
        {
            pHero = Warrior::create("unit_warrior.png");
            break;
        }
        case Hero::Type::MAGE:
        {
            pHero = Mage::create("unit_mage.png");
            break;
        }
        case Hero::Type::ROGUE:
        {
            pHero = Rogue::create("unit_rogue.png");
            break;
        }
        case Hero::Type::PRIEST:
        {
            pHero = Priest::create("unit_priest.png");
            break;
        }
        default:
            assert(false);
            break;
    }
    pHero->SetUID(player.nUID);
    pHero->SetName(player.sNickname);
    pHero->SetGameWorld(this);
    m_apoObjects.push_back(pHero);
    this->addChild(pHero, 10);
    if(player.nUID == GameConfiguraton::Instance().GetUID())
    {
        m_poLocalPlayer = pHero;
        pHero->SetIsLocalPlayer(true);
    }
}

void
GameWorld::ReceiveInputNetEvents()
{
    auto& socket = NetSystem::Instance().GetChannel(1);
    unsigned char buf[512];
    
    while(socket.DataAvailable())
    {
        socket.ReceiveBytes();
        
        auto gs_event = GameEvent::GetMessage(socket.GetBuffer().data());
        
        switch(gs_event->event_type())
        {
            case GameEvent::Events_SVSpawnPlayer:
            {
                auto gs_spawn = static_cast<const GameEvent::SVSpawnPlayer*>(gs_event->event());
                for(auto object : m_apoObjects)
                {
                    if(object->GetObjType() == GameObject::Type::UNIT)
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
                
            case GameEvent::Events_SVSpawnMonster:
            {
                auto gs_spawn = static_cast<const GameEvent::SVSpawnMonster*>(gs_event->event());
                
                auto monster = Monster::create("unit_skeleton.png");
                monster->SetGameWorld(this);
                monster->SetUID(gs_spawn->monster_uid());
                monster->Spawn(cocos2d::Vec2(gs_spawn->x(),
                                             gs_spawn->y()));
                m_apoObjects.push_back(monster);
                this->addChild(monster, 10);
                
                m_pUI->m_pBattleLogs->AddLogMessage("Skeleton spawned");
                
                break;
            }
                
            case GameEvent::Events_SVSpawnItem:
            {
                auto gs_spawn = static_cast<const GameEvent::SVSpawnItem*>(gs_event->event());
                
                switch((Item::Type)gs_spawn->item_type())
                {
                    case Item::Type::KEY:
                    {
                        auto key = Key::create("item_key.png");
                        key->SetUID(gs_spawn->item_uid());
                        key->SetCarrierID(0);
                        
                        cocos2d::Vec2 log_coords(gs_spawn->x(),
                                                 gs_spawn->y());
                        cocos2d::Vec2 spritePos = LOG_TO_PHYS_COORD(log_coords,
                                                                    key->getContentSize());
                        
                        key->SetGameWorld(this);
                        key->SetLogicalPosition(log_coords);
                        key->setPosition(spritePos);
                        m_apoObjects.push_back(key);
                        this->addChild(key, 0);
                        
                        key->AnimationSpawn();
                        break;
                    }
                        
                    default:
                        assert(false);
                        break;
                }
                
                break;
            }
                
            case GameEvent::Events_SVSpawnConstr:
            {
                auto gs_spawn = static_cast<const GameEvent::SVSpawnConstr*>(gs_event->event());
                
                switch(gs_spawn->constr_type())
                {
                    case Construction::Type::GRAVEYARD:
                    {
                        auto grave = Graveyard::create("construction_graveyard.png");
                        
                        cocos2d::Vec2 log_coords(gs_spawn->x(),
                                                 gs_spawn->y());
                        cocos2d::Vec2 spritePos = LOG_TO_PHYS_COORD(log_coords,
                                                                    grave->getContentSize());
                        
                        grave->SetLogicalPosition(log_coords);
                        grave->setPosition(spritePos);
                        m_apoObjects.push_back(grave);
                        this->addChild(grave, 0);
                        break;
                    }
                        
                    case Construction::Type::DOOR:
                    {
                        auto door = Door::create("construction_door.png");
                        
                        cocos2d::Vec2 log_coords(gs_spawn->x(),
                                                 gs_spawn->y());
                        cocos2d::Vec2 spritePos = LOG_TO_PHYS_COORD(log_coords,
                                                                    door->getContentSize());
                        
                        door->SetLogicalPosition(log_coords);
                        door->setPosition(spritePos);
                        m_apoObjects.push_back(door);
                        this->addChild(door, 0);
                        break;
                    }
                        
                    default:
                        assert(false);
                        break;
                }
                
                break;
            }
                
            case GameEvent::Events_SVActionMove:
            {
                auto gs_mov = static_cast<const GameEvent::SVActionMove*>(gs_event->event());
                
                for(auto object : m_apoObjects)
                {
                    if(object->GetObjType() == GameObject::Type::UNIT)
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
                
            case GameEvent::Events_SVActionItem:
            {
                auto gs_item = static_cast<const GameEvent::SVActionItem*>(gs_event->event());
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
                    case GameEvent::ActionItemType_TAKE:
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
                
            case GameEvent::Events_SVActionDuel:
            {
                auto sv_duel = static_cast<const GameEvent::SVActionDuel*>(gs_event->event());
                
                switch(sv_duel->act_type())
                {
                    case GameEvent::ActionDuelType_STARTED:
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
                
            case GameEvent::Events_SVActionDeath:
            {
                auto gs_death = static_cast<const GameEvent::SVActionDeath*>(gs_event->event());
                
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
                
            case GameEvent::Events_SVRespawnPlayer:
            {
                auto gs_resp = static_cast<const GameEvent::SVRespawnPlayer*>(gs_event->event());
                
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
                
            case GameEvent::Events_SVGameEnd:
            {
                auto gs_go = static_cast<const GameEvent::SVGameEnd*>(gs_event->event());
                
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
                
            case GameEvent::Events_SVActionSpell:
            {
                auto gs_spell = static_cast<const GameEvent::SVActionSpell*>(gs_event->event());
                
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
        NetSystem::Instance().GetChannel(1).SendBytes(event.data(),
                                                      event.size());
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
