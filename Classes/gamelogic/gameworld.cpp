//
//  gameworld.cpp
//  labyrinth
//
//  Created by Aleksandr Borzikh on 15.04.17.
//
//

#include "gameworld.hpp"

#include "../netsystem.hpp"
#include "item.hpp"
#include "construction.hpp"
#include "units/units_inc.hpp"

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
    switch(player.nHeroIndex)
    {
        case Hero::Type::AIR_ELEMENTALIST:
        {
            auto air_elem = AirElementalist::create("res/units/player_down.png");
            air_elem->SetUID(player.nUID);
            air_elem->SetName(player.sNickname);
            air_elem->SetGameWorld(this);
            m_apoObjects.push_back(air_elem);
            this->addChild(air_elem, 1);
            
            if(player.nUID == GameConfiguraton::Instance().GetUID())
                m_poLocalPlayer = air_elem;
            break;
        }
        case Hero::Type::EARTH_ELEMENTALIST:
        {
            auto earth_elem = EarthElementalist::create("res/units/player_down.png");
            earth_elem->SetUID(player.nUID);
            earth_elem->SetName(player.sNickname);
            earth_elem->SetGameWorld(this);
            m_apoObjects.push_back(earth_elem);
            this->addChild(earth_elem, 1);
            
            if(player.nUID == GameConfiguraton::Instance().GetUID())
                m_poLocalPlayer = earth_elem;
            break;
        }
        case Hero::Type::WATER_ELEMENTALIST:
        {
            auto water_elem = WaterElementalist::create("res/units/player_down.png");
            water_elem->SetUID(player.nUID);
            water_elem->SetName(player.sNickname);
            water_elem->SetGameWorld(this);
            m_apoObjects.push_back(water_elem);
            this->addChild(water_elem, 1);
            
            if(player.nUID == GameConfiguraton::Instance().GetUID())
                m_poLocalPlayer = water_elem;
            break;
        }
        case Hero::Type::FIRE_ELEMENTALIST:
        {
            auto fire_elem = FireElementalist::create("res/units/player_down.png");
            fire_elem->SetUID(player.nUID);
            fire_elem->SetName(player.sNickname);
            fire_elem->SetGameWorld(this);
            m_apoObjects.push_back(fire_elem);
            this->addChild(fire_elem, 1);
            
            if(player.nUID == GameConfiguraton::Instance().GetUID())
                m_poLocalPlayer = fire_elem;
            break;
        }
        default:
            assert(false);
            break;
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
                
                auto monster = Monster::create("res/units/monster.png");
                monster->SetGameWorld(this);
                monster->SetUID(gs_spawn->monster_uid());
                monster->Spawn(cocos2d::Vec2(gs_spawn->x(),
                                             gs_spawn->y()));
                m_apoObjects.push_back(monster);
                this->addChild(monster, 1);
                break;
            }
                
            case GameEvent::Events_SVSpawnItem:
            {
                auto gs_spawn = static_cast<const GameEvent::SVSpawnItem*>(gs_event->event());
                
                switch((Item::Type)gs_spawn->item_type())
                {
                    case Item::Type::KEY:
                    {
                        auto key = Key::create("res/key.png");
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
                        
                    case Item::Type::SWORD:
                    {
                        auto sword = Sword::create("res/sword.png");
                        sword->SetUID(gs_spawn->item_uid());
                        sword->SetCarrierID(0);
                        
                        cocos2d::Vec2 log_coords(gs_spawn->x(),
                                                 gs_spawn->y());
                        cocos2d::Vec2 spritePos = LOG_TO_PHYS_COORD(log_coords,
                                                                    sword->getContentSize());
                        
                        sword->SetGameWorld(this);
                        sword->SetLogicalPosition(log_coords);
                        sword->setPosition(spritePos);
                        m_apoObjects.push_back(sword);
                        this->addChild(sword, 0);
                        
                        sword->AnimationSpawn();
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
                    case Construction::Type::SWAMP:
                    {
                        auto swamp = Swamp::create("res/swamp.png");
                        
                        cocos2d::Vec2 log_coords(gs_spawn->x(),
                                                 gs_spawn->y());
                        cocos2d::Vec2 spritePos = LOG_TO_PHYS_COORD(log_coords,
                                                                    swamp->getContentSize());
                        
                        swamp->SetLogicalPosition(log_coords);
                        swamp->setPosition(spritePos);
                        m_apoObjects.push_back(swamp);
                        this->addChild(swamp, 0);
                        break;
                    }
                        
                    case Construction::Type::GRAVEYARD:
                    {
                        auto grave = Graveyard::create("res/graveyard.png");
                        
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
                        auto door = Door::create("res/door.png");
                        
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
                            unit->Move(cocos2d::Vec2(gs_mov->x(),
                                                     gs_mov->y()));
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
                        break;
                    }
                        
                    case GameEvent::ActionItemType_DROP:
                    {
                        item->AnimationDropped();
                        item->SetCarrierID(0);
                        item->SetLogicalPosition(player->GetLogicalPosition());
                        item->setPosition(
                                          LOG_TO_PHYS_COORD(player->GetLogicalPosition(),
                                                            item->getContentSize()));
                        
                            // delete item from players inventory
                        for(auto it = player->GetInventory().begin();
                            it != player->GetInventory().end();
                            ++it)
                        {
                            if((*it)->GetUID() == item->GetUID())
                            {
                                player->GetInventory().erase(it);
                                break;
                            }
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
                        
                    case GameEvent::ActionDuelType_ATTACK:
                    {
                        Unit * attacker = nullptr;
                        
                        for(auto object : m_apoObjects)
                        {
                            if(object->GetUID() == sv_duel->target1_uid())
                            {
                                attacker = static_cast<Unit*>(object);
                            }
                        }
                        
                        attacker->Attack();
                        break;
                    }
                        
                    case GameEvent::ActionDuelType_KILL:
                    {
                            // player killed
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
                        
                        first->EndDuel();
                        second->EndDuel();
                        second->Die();
                        
                        m_qBattleLogs.push(cocos2d::StringUtils::format("%s killed %s",
                                                                        first->GetName().c_str(),
                                                                        second->GetName().c_str()));
                        break;
                    }
                        
                    default:
                        assert(false);
                        break;
                }
                
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
                this->release();
                cocos2d::Director::getInstance()->popScene();
                
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
                        if(gs_spell->spell_id() == 1)
                        {
                            player->SpellCast1();
                        }
                        else
                        {
                            assert(false);
                        }
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
