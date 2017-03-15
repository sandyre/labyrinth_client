    //
    //  gamescene.cpp
    //  labyrinth
    //
    //  Created by Aleksandr Borzikh on 12.01.17.
    //
    //

#include "gamescene.hpp"

#include "globals.h"
#include "playerinfo.hpp"

#include <memory>
#include <SimpleAudioEngine.h>
#include "mainmenuscene.hpp"
#include "netsystem.hpp"
#include "gsnet_generated.h"

#include "gamelogic/units/units_inc.hpp"

USING_NS_CC;

GameScene::GameScene() :
m_pGameHUD(new GameHUD()),
m_pPlayersLayer(new Layer()),
m_pItemsLayer(new Layer()),
m_pSwampCombo(new SwampCombo()),
m_pDuelMode(new DuelMode())
{
    this->autorelease();
}

GameScene::~GameScene()
{
    
}

void
GameScene::onExit()
{
    Layer::onExit();
}

/*
 * BUILDER API
 */
void
GameScene::GenerateMap(const GameMap::Configuration &conf)
{
    m_oGameMap.GenerateMap(conf);
}

std::vector<Hero*>&
GameScene::GetPlayersList()
{
    return m_aPlayers;
}

bool
GameScene::init()
{
    if( !Layer::init() )
    {
        return false;
    }
    
    auto visibleSize = Director::getInstance()->getVisibleSize();
    
        // set local player
    m_pLocalPlayer = GetPlayerByUID(PlayerInfo::Instance().GetUID());
    
        // order matters!
    m_oGameMap.GetFloorLayer()->setPosition(Vec2::ZERO);
    this->addChild(m_oGameMap.GetFloorLayer());
    m_oGameMap.GetWallsLayer()->setPosition(Vec2::ZERO);
    this->addChild(m_oGameMap.GetWallsLayer());
    
    m_pItemsLayer->setPosition(Vec2::ZERO);
    this->addChild(m_pItemsLayer);
    
    m_pPlayersLayer->setPosition(Vec2::ZERO);
    this->addChild(m_pPlayersLayer);
    
    m_pGameHUD->setCameraMask((unsigned short)CameraFlag::USER1);
    m_pGameHUD->setPosition(Vec2::ZERO);
    this->addChild(m_pGameHUD);
    
    auto hud_camera = Camera::create();
    hud_camera->setCameraFlag(CameraFlag::USER1);
    this->addChild(hud_camera);
    
        // prepare SwampCombo action
    m_pSwampCombo->setCameraMask((unsigned short)CameraFlag::USER1);
    m_pSwampCombo->setPosition(Vec2::ZERO);
    m_pSwampCombo->Show(false);
    m_pGameHUD->addChild(m_pSwampCombo);
    
        // prepare duelmode action
    m_pDuelMode->setCameraMask((unsigned short)CameraFlag::USER1);
    m_pDuelMode->setPosition(Vec2::ZERO);
    m_pDuelMode->Show(false);
    m_pGameHUD->addChild(m_pDuelMode);
    
    auto eventListener = EventListenerKeyboard::create();
    eventListener->onKeyPressed = [this](EventKeyboard::KeyCode keyCode, Event * event)
    {
        if(m_pLocalPlayer->GetState() == Unit::State::WALKING)
        {
            auto pos = m_pLocalPlayer->GetLogicalPosition();
            auto pCam = Camera::getDefaultCamera();
            auto camPos = pCam->getPosition3D();
            switch(keyCode)
            {
                case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
                {
                    if(pos.x - 1 >= 0 &&
                       m_oGameMap[pos.x-1][pos.y]->GetType() ==
                       MapBlock::Type::NOBLOCK)
                    {
                        --pos.x;
                    }
                    m_pLocalPlayer->Turn(Hero::Orientation::LEFT);
                    break;
                }
                    
                case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
                {
                    if(pos.x + 1 < m_oGameMap[0].size() &&
                       m_oGameMap[pos.x+1][pos.y]->GetType() ==
                       MapBlock::Type::NOBLOCK)
                    {
                        ++pos.x;
                    }
                    m_pLocalPlayer->Turn(Hero::Orientation::RIGHT);
                    break;
                }
                    
                case EventKeyboard::KeyCode::KEY_UP_ARROW:
                {
                    if(pos.y + 1 < m_oGameMap[0].size() &&
                       m_oGameMap[pos.x][pos.y+1]->GetType() ==
                       MapBlock::Type::NOBLOCK)
                    {
                        ++pos.y;
                    }
                    m_pLocalPlayer->Turn(Hero::Orientation::UP);
                    break;
                }
                    
                case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
                {
                    if(pos.y - 1 >= 0 &&
                       m_oGameMap[pos.x][pos.y-1]->GetType() ==
                       MapBlock::Type::NOBLOCK)
                    {
                        --pos.y;
                    }
                    m_pLocalPlayer->Turn(Hero::Orientation::DOWN);
                    break;
                }
                case EventKeyboard::KeyCode::KEY_T:
                {
                    auto item = std::find_if(m_aItems.begin(),
                                             m_aItems.end(),
                                             [this](Item * item)
                                             {
                                                 return item->GetLogicalPosition() == m_pLocalPlayer->GetLogicalPosition() &&
                                                 item->GetCarrierID() == 0;
                                             });
                    if(item != m_aItems.end())
                    {
                        auto event = m_pLocalPlayer->EventItemTake((*item)->GetUID());
                        m_aOutEvents.push(event);
                        return;
                    }
                    break;
                }
                case EventKeyboard::KeyCode::KEY_Q:
                {
                    if(m_pLocalPlayer->isSpellCast1Ready())
                    {
                        m_aOutEvents.push(m_pLocalPlayer->EventSpellCast1());
                    }
                    break;
                }
            }
            
            auto event = m_pLocalPlayer->EventMove(pos);
            if(!event.empty())
            {
                m_aOutEvents.push(event);
            }
        }
        else if(m_pLocalPlayer->GetState() == Unit::State::SWAMP)
        {
            switch(keyCode)
            {
                case EventKeyboard::KeyCode::KEY_UP_ARROW:
                    m_pSwampCombo->TakeInput(InputMove::UP);
                    break;
                case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
                    m_pSwampCombo->TakeInput(InputMove::DOWN);
                    break;
                case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
                    m_pSwampCombo->TakeInput(InputMove::LEFT);
                    break;
                case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
                    m_pSwampCombo->TakeInput(InputMove::RIGHT);
                    break;
            }
        }
        else if(m_pLocalPlayer->GetState() == Unit::State::DUEL)
        {
            switch(keyCode)
            {
                case EventKeyboard::KeyCode::KEY_UP_ARROW:
                    m_pDuelMode->TakeInput(InputMove::UP);
                    break;
                case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
                    m_pDuelMode->TakeInput(InputMove::DOWN);
                    break;
                case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
                    m_pDuelMode->TakeInput(InputMove::LEFT);
                    break;
                case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
                    m_pDuelMode->TakeInput(InputMove::RIGHT);
                    break;
            }
        }
    };
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(eventListener, this);
    
    auto eventListenerTouch = EventListenerTouchOneByOne::create();
    eventListenerTouch->onTouchBegan = [this](Touch * touch, Event * event)
    {
        if(m_pGameHUD->m_pTakeItem->getBoundingBox().containsPoint(touch->getLocation()))
        {
            if(!m_pGameHUD->m_pTakeItem->isVisible())
            {
                return false;
            }
            
            auto item = std::find_if(m_aItems.begin(),
                                     m_aItems.end(),
                                     [this](Item * item)
                                     {
                                         return item->GetLogicalPosition() == m_pLocalPlayer->GetLogicalPosition() &&
                                         item->GetCarrierID() == 0;
                                     });
            auto event = m_pLocalPlayer->EventItemTake((*item)->GetUID());
            m_aOutEvents.push(event);
            
            return true;
        }
        
        return false;
    };
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(eventListenerTouch, this);
    
    this->scheduleUpdate();
    return true;
}

void
GameScene::update(float delta)
{
    SendOutputEvents();
    ApplyInputEvents();
    UpdateView(delta);
    
        // check that player can enter duel
    for(auto player : m_aPlayers)
    {
        auto pos1 = player->GetLogicalPosition();
        auto pos2 = m_pLocalPlayer->GetLogicalPosition();
        if((m_pLocalPlayer->GetStatus() & Hero::Status::DUELABLE) &&
           (player->GetStatus() & Hero::Status::DUELABLE) &&
           ((std::abs(pos1.x - pos2.x) + std::abs(pos1.y - pos2.y)) == 1))
        {
            auto event = m_pLocalPlayer->EventDuelStart(player->GetUID());
            m_aOutEvents.push(event);
        }
    }
}

Item *
GameScene::GetItemByUID(uint16_t uid)
{
    for(size_t i = 0;
        i < m_aItems.size();
        ++i)
    {
        if(m_aItems[i]->GetUID() == uid)
            return m_aItems[i];
    }
    
    return nullptr;
}

Hero *
GameScene::GetPlayerByUID(uint32_t uid)
{
    for(size_t i = 0;
        i < m_aPlayers.size();
        ++i)
    {
        if(m_aPlayers[i]->GetUID() == uid)
            return m_aPlayers[i];
    }
    
    return nullptr;
}

Monster *
GameScene::GetMonsterByUID(uint16_t uid)
{
    for(size_t i = 0;
        i < m_aMonsters.size();
        ++i)
    {
        if(m_aMonsters[i]->GetUID() == uid)
            return m_aMonsters[i];
    }
    
    return nullptr;
}

void
GameScene::UpdateView(float delta)
{
        // representation is based on state
    if(m_pLocalPlayer->GetState() == Hero::State::WALKING)
    {
            // check that player can take some item
        for(auto& item : m_aItems)
        {
            if(item->GetCarrierID() == 0 &&
               item->GetLogicalPosition() == m_pLocalPlayer->GetLogicalPosition())
            {
                m_pGameHUD->m_pTakeItem->setVisible(true);
                break;
            }
            m_pGameHUD->m_pTakeItem->setVisible(false);
        }
        
        auto pCam = Camera::getDefaultCamera();
        auto cur_pos = pCam->getPosition3D();
        cur_pos.x = (m_pLocalPlayer->getPosition().x - cur_pos.x)*delta*3 + cur_pos.x;
        cur_pos.y = (m_pLocalPlayer->getPosition().y- cur_pos.y)*delta*3 + cur_pos.y;
        pCam->setPosition3D(Vec3(
                                 cur_pos.x,
                                 cur_pos.y,
                                 800));
    }
    else if(m_pLocalPlayer->GetState() == Hero::State::SWAMP)
    {
        if(m_pSwampCombo->ComboDone())
        {
            m_pSwampCombo->Reset();
            
            auto cl_escape = GameEvent::CreateCLActionSwamp(builder,
                                                            m_pLocalPlayer->GetUID(),
                                                            GameEvent::ActionSwampStatus_ESCAPED);
            auto gs_event = GameEvent::CreateMessage(builder,
                                                   GameEvent::Events_CLActionSwamp,
                                                   cl_escape.Union());
            builder.Finish(gs_event);
            m_aOutEvents.push(std::vector<char>(builder.GetBufferPointer(),
                                                builder.GetBufferPointer() + builder.GetSize()));
        }
    }
    else if(m_pLocalPlayer->GetState() == Unit::State::DUEL)
    {
        if(m_pDuelMode->ComboDone())
        {
            m_pDuelMode->Reset();
            auto event = m_pLocalPlayer->EventDuelAttack();
            m_aOutEvents.push(event);
        }
        
        auto enemy = m_pLocalPlayer->GetDuelTarget();
        m_pDuelMode->SetEnemyInfo(enemy->GetName(),
                                  enemy->GetHealth());
    }
    else if(m_pLocalPlayer->GetState() == Hero::State::DEAD)
    {
        
    }
        // update hud
    m_pGameHUD->m_pHP->setString(StringUtils::format("Health: %d/%d",
                                                     m_pLocalPlayer->GetHealth(),
                                                     m_pLocalPlayer->GetMaxHealth()));
    m_pGameHUD->m_pDMG->setString(StringUtils::format("DMG: %d",
                                                      m_pLocalPlayer->GetDamage()));
    
    std::string inventory = "Inventory:\n";
    for(auto item : m_pLocalPlayer->GetInventory())
    {
        if(item->GetType() == Item::Type::KEY)
            inventory += "Key\n";
        else if(item->GetType() == Item::Type::SWORD)
            inventory += "Sword\n";
    }
    m_pGameHUD->m_pInventory->setString(inventory);
}

void
GameScene::ApplyInputEvents()
{
    auto& socket = NetSystem::Instance().Socket();
    unsigned char buf[512];
    while(socket.available())
    {
        socket.receiveBytes(buf, 512);
        
        auto gs_event = GameEvent::GetMessage(buf);
        
        switch(gs_event->event_type())
        {
            case GameEvent::Events_SVSpawnPlayer:
            {
                auto gs_spawn = static_cast<const GameEvent::SVSpawnPlayer*>(gs_event->event());
                for(auto player : m_aPlayers)
                {
                    if(player->GetUID() == gs_spawn->player_uid())
                    {
                        cocos2d::Vec2 log_coords(gs_spawn->x(),
                                                 gs_spawn->y());
                        player->Spawn(log_coords);
                        m_pPlayersLayer->addChild(player);
                        break;
                    }
                }
                
                break;
            }
                
            case GameEvent::Events_SVSpawnMonster:
            {
                auto gs_spawn = static_cast<const GameEvent::SVSpawnMonster*>(gs_event->event());
                
                auto monster = Monster::create("res/units/monster.png");
                monster->SetUID(gs_spawn->monster_uid());
                
                cocos2d::Vec2 log_coords(gs_spawn->x(),
                                         gs_spawn->y());
                
                monster->Spawn(log_coords);
                m_pPlayersLayer->addChild(monster);
                m_aMonsters.push_back(monster);
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
                        
                        key->SetLogicalPosition(log_coords);
                        key->setPosition(spritePos);
                        m_oGameMap.GetFloorLayer()->addChild(key);
                        
                        key->AnimationSpawn();
                        
                        m_aItems.push_back(key);
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
                        
                        sword->SetLogicalPosition(log_coords);
                        sword->setPosition(spritePos);
                        
                        m_oGameMap.GetFloorLayer()->addChild(sword);
                        
                        sword->AnimationSpawn();
                        
                        m_aItems.push_back(sword);
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
                        
                        m_oGameMap.GetFloorLayer()->addChild(swamp);
                        
                        m_aConstrs.push_back(swamp);
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
                        
                        m_oGameMap.GetFloorLayer()->addChild(grave);
                        
                        m_aConstrs.push_back(grave);
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
                        
                        m_oGameMap.GetFloorLayer()->addChild(door);
                        
                        m_aConstrs.push_back(door);
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
                
                if(gs_mov->target_type() == GameEvent::ActionMoveTarget_PLAYER)
                {
                    for(auto player : m_aPlayers)
                    {
                        if(player->GetUID() == gs_mov->target_uid())
                        {
                            auto new_pos = cocos2d::Vec2(gs_mov->x(), gs_mov->y());
                            
                            if(player->GetLogicalPosition().x > new_pos.x)
                            {
                                player->Turn(Hero::Orientation::LEFT);
                            }
                            else if(player->GetLogicalPosition().x < new_pos.x)
                            {
                                player->Turn(Hero::Orientation::RIGHT);
                            }
                            else if(player->GetLogicalPosition().y < new_pos.y)
                            {
                                player->Turn(Hero::Orientation::UP);
                            }
                            else if(player->GetLogicalPosition().y > new_pos.y)
                            {
                                player->Turn(Hero::Orientation::DOWN);
                            }
                            
                            player->Move(new_pos);
                            break;
                        }
                    }
                }
                else if(gs_mov->target_type() == GameEvent::ActionMoveTarget_MONSTER)
                {
                    for(auto& monster : m_aMonsters)
                    {
                        if(monster->GetUID() == gs_mov->target_uid())
                        {
                            auto new_pos = cocos2d::Vec2(gs_mov->x(),
                                                         gs_mov->y());
                            
                            monster->Move(new_pos);
                            break;
                        }
                    }
                }
                
                break;
            }
                
            case GameEvent::Events_SVActionItem:
            {
                auto gs_item = static_cast<const GameEvent::SVActionItem*>(gs_event->event());
                auto item = GetItemByUID(gs_item->item_uid());
                auto player = GetPlayerByUID(gs_item->player_uid());
                
                switch(gs_item->act_type())
                {
                    case GameEvent::ActionItemType_TAKE:
                    {
                        item->AnimationTaken();
                        item->SetCarrierID(gs_item->player_uid());
                        player->TakeItem(item);
                        player->UpdateStats();
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
                        player->UpdateStats();
                        
                        break;
                    }
                        
                    default:
                        assert(false);
                        break;
                }
                
                break;
            }
                
            case GameEvent::Events_SVActionSwamp:
            {
//                auto gs_swamp = static_cast<const GameEvent::SVActionSwamp*>(gs_event->event());
//                
//                switch(gs_swamp->status())
//                {
//                    case GameEvent::ActionSwampStatus_STARTED:
//                    {
//                        for(auto player : m_aPlayers)
//                        {
//                            if(player->GetUID() == gs_swamp->player_uid())
//                            {
//                                player->SetState(Hero::State::SWAMP);
//                                
//                                if(player->GetUID() == m_pLocalPlayer->GetUID())
//                                {
//                                    m_pSwampCombo->Show(true);
//                                }
//                                break;
//                            }
//                        }
//                        
//                        break;
//                    }
//                        
//                    case GameEvent::ActionSwampStatus_DIED:
//                    {
//                        for(auto player : m_aPlayers)
//                        {
//                            if(player->GetUID() == gs_swamp->player_uid())
//                            {
//                                player->SetState(Hero::State::DEAD);
//                                player->AnimationDeath();
//                                
//                                if(player->GetUID() == gs_swamp->player_uid())
//                                {
//                                    m_pSwampCombo->Show(false);
//                                    m_pSwampCombo->Reset();
//                                }
//                                break;
//                            }
//                        }
//                        
//                        break;
//                    }
//                        
//                    case GameEvent::ActionSwampStatus_ESCAPED:
//                    {
//                        for(auto player : m_aPlayers)
//                        {
//                            if(player->GetUID() == gs_swamp->player_uid())
//                            {
//                                player->SetState(Hero::State::WALKING);
//                                
//                                if(player->GetUID() == m_pLocalPlayer->GetUID())
//                                {
//                                    m_pSwampCombo->Show(false);
//                                    m_pSwampCombo->Reset();
//                                }
//                                
//                                break;
//                            }
//                        }
//                        
//                        break;
//                    }
//                        
//                    default:
//                        assert(false);
//                        break;
//                }
//                
//                break;
            }
                
            case GameEvent::Events_SVActionDuel:
            {
                auto sv_duel = static_cast<const GameEvent::SVActionDuel*>(gs_event->event());
                
                switch(sv_duel->act_type())
                {
                    case GameEvent::ActionDuelType_STARTED:
                    {
                            // p v p
                        if(sv_duel->target2_type() == GameEvent::ActionDuelTarget_PLAYER)
                        {
                            auto player1 = GetPlayerByUID(sv_duel->target1_uid());
                            auto player2 = GetPlayerByUID(sv_duel->target2_uid());
                            
                            player1->StartDuel(player2);
                            player2->StartDuel(player1);
                            
                            if(m_pLocalPlayer->GetUID() == player1->GetUID())
                            {
                                m_pDuelMode->Show(true);
                            }
                            
                            if(m_pLocalPlayer->GetUID() == player2->GetUID())
                            {
                                m_pDuelMode->Show(true);
                            }
                        }
                            // p v m
                        else if(sv_duel->target2_type() == GameEvent::ActionDuelTarget_MONSTER)
                        {
                            auto player1 = GetPlayerByUID(sv_duel->target1_uid());
                            auto monster = GetMonsterByUID(sv_duel->target2_uid());
                            
                            player1->StartDuel(monster);
                            monster->StartDuel(player1);
                            
                            if(m_pLocalPlayer->GetUID() == player1->GetUID())
                            {
                                m_pDuelMode->Show(true);
                            }
                        }
                        
                        break;
                    }
                        
                    case GameEvent::ActionDuelType_ATTACK:
                    {
                            // player attacks
                        if(sv_duel->target1_type() == GameEvent::ActionDuelTarget_PLAYER)
                        {
                            auto player1 = GetPlayerByUID(sv_duel->target1_uid());
                            player1->Attack();
                        }
                            // monster attacks
                        else if(sv_duel->target1_type() == GameEvent::ActionDuelTarget_MONSTER)
                        {
                            auto monster = GetMonsterByUID(sv_duel->target1_uid());
                            monster->Attack();
                        }
                        
                        break;
                    }
                        
                    case GameEvent::ActionDuelType_KILL:
                    {
                            // player killed
                        if(sv_duel->target1_type() == GameEvent::ActionDuelTarget_PLAYER)
                        {
                            if(sv_duel->target2_type() == GameEvent::ActionDuelTarget_PLAYER)
                            {
                                auto player1 = GetPlayerByUID(sv_duel->target1_uid());
                                auto player2 = GetPlayerByUID(sv_duel->target2_uid());
                                
                                player1->EndDuel();
                                
                                if(m_pLocalPlayer->GetUID() == player1->GetUID())
                                {
                                    m_pDuelMode->Show(false);
                                    m_pDuelMode->Reset();
                                }
                                
                                player2->EndDuel();
                                player2->Die();
                                
                                if(m_pLocalPlayer->GetUID() == player2->GetUID())
                                {
                                    m_pDuelMode->Show(false);
                                    m_pDuelMode->Reset();
                                }
                            }
                            else if(sv_duel->target2_type() == GameEvent::ActionDuelTarget_MONSTER)
                            {
                                auto player = GetPlayerByUID(sv_duel->target1_uid());
                                auto monster = GetMonsterByUID(sv_duel->target2_uid());
                                
                                player->EndDuel();
                                if(m_pLocalPlayer->GetUID() == player->GetUID())
                                {
                                    m_pDuelMode->Show(false);
                                    m_pDuelMode->Reset();
                                }
                                
                                monster->EndDuel();
                                monster->Die();
                            }
                        }
                            // monster killed player
                        else if(sv_duel->target1_type() == GameEvent::ActionDuelTarget_MONSTER)
                        {
                            auto monster = GetMonsterByUID(sv_duel->target1_uid());
                            auto player = GetPlayerByUID(sv_duel->target2_uid());
                            
                            monster->EndDuel();
                            
                            player->EndDuel();
                            player->Die();
                            
                            if(m_pLocalPlayer->GetUID() == player->GetUID())
                            {
                                m_pDuelMode->Show(false);
                                m_pDuelMode->Reset();
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
                
            case GameEvent::Events_SVRespawnPlayer:
            {
                auto gs_resp = static_cast<const GameEvent::SVRespawnPlayer*>(gs_event->event());
                
                for(auto player : m_aPlayers)
                {
                    if(player->GetUID() == gs_resp->player_uid())
                    {
                        cocos2d::Vec2 log_coords(gs_resp->x(),
                                                 gs_resp->y());
                        player->Respawn(log_coords);
                        break;
                    }
                }
                
                break;
            }
                
            case GameEvent::Events_SVGameEnd:
            {
                auto gs_go = static_cast<const GameEvent::SVGameEnd*>(gs_event->event());
                this->release();
                Director::getInstance()->replaceScene(MainMenuScene::createScene());
                
                break;
            }
                
            case GameEvent::Events_SVActionSpell:
            {
                auto gs_spell = static_cast<const GameEvent::SVActionSpell*>(gs_event->event());
                auto player = GetPlayerByUID(gs_spell->player_uid());
                
                if(player->GetHero() == Hero::Type::AIR_ELEMENTALIST)
                {
                    auto air = static_cast<AirElementalist*>(player);
                    air->SpellCast1();
                }
                else if(player->GetHero() == Hero::Type::WATER_ELEMENTALIST)
                {
                    auto water = static_cast<WaterElementalist*>(player);
                    water->SpellCast1();
                }
                else if(player->GetHero() == Hero::Type::EARTH_ELEMENTALIST)
                {
                    auto earth = static_cast<EarthElementalist*>(player);
                    earth->SpellCast1();
                    
                    auto& block = m_oGameMap[gs_spell->point_x()][gs_spell->point_y()];
                    auto scale = cocos2d::ScaleBy::create(1.0, 0.1);
                    auto fade = cocos2d::FadeOut::create(1.0);
                    auto spawn = cocos2d::Spawn::create(scale, fade, nullptr);
                    block->runAction(spawn);
                    block = NoBlock::create("res/floor.png");
                    block->SetLogicalPosition(Vec2(gs_spell->point_x(),
                                                   gs_spell->point_y()));
                    block->setPosition(LOG_TO_PHYS_COORD(Vec2(gs_spell->point_x(),
                                                              gs_spell->point_y()),
                                                         block->getContentSize()));
                    m_oGameMap.GetFloorLayer()->addChild(block);
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
GameScene::SendOutputEvents()
{
    while(!m_aOutEvents.empty())
    {
        NetSystem::Instance().Socket().sendBytes(m_aOutEvents.front().data(),
                                                 m_aOutEvents.front().size());
        m_aOutEvents.pop();
    }
}
