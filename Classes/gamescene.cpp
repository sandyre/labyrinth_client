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

std::vector<Player*>&
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
        if(m_pLocalPlayer->GetState() == Player::State::WALKING)
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
                    break;
                }
            }
            
            if(!m_pLocalPlayer->getNumberOfRunningActions())
            {
                auto mov = GameEvent::CreateCLActionMove(builder,
                                                         m_pLocalPlayer->GetUID(),
                                                         GameEvent::ActionMoveTarget_PLAYER,
                                                         pos.x,
                                                         pos.y);
                auto gs_event = GameEvent::CreateEvent(builder,
                                                       GameEvent::Events_CLActionMove,
                                                       mov.Union());
                builder.Finish(gs_event);
                SendEventAndClear();
            }
        }
        else if(m_pLocalPlayer->GetState() == Player::State::SWAMP)
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
        else if(m_pLocalPlayer->GetState() == Player::State::DUEL_PLAYER ||
                m_pLocalPlayer->GetState() == Player::State::DUEL_MONSTER)
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
            auto cl_take = GameEvent::CreateCLActionItem(builder,
                                                         m_pLocalPlayer->GetUID(),
                                                         (*item)->GetUID(),
                                                         GameEvent::ActionItemType_TAKE);
            auto gs_event = GameEvent::CreateEvent(builder,
                                                   GameEvent::Events_CLActionItem,
                                                   cl_take.Union());
            builder.Finish(gs_event);
            SendEventAndClear();
            
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
        // music
    auto audio = CocosDenshion::SimpleAudioEngine::getInstance();
//    if(!audio->isBackgroundMusicPlaying())
//    {
//        audio->setEffectsVolume(0.2);
//        audio->setBackgroundMusicVolume(0.1);
//        audio->playBackgroundMusic("res/background.mp3");
//    }
    
        // apply received events first
    auto& socket = NetSystem::Instance().Socket();
    unsigned char buf[512];
    while(socket.available())
    {
        socket.receiveBytes(buf, 512);
        
        auto gs_event = GameEvent::GetEvent(buf);
        
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
                        player->SetLogicalPosition(log_coords);
                        player->SetHealth(gs_spawn->hp());
                        player->SetMaxHealth(gs_spawn->max_hp());
                        
                        cocos2d::Vec2 spritePos = LOG_TO_PHYS_COORD(log_coords,
                                                                    player->getContentSize());
                        player->setPosition(spritePos);
                        m_pPlayersLayer->addChild(player);
                        
                        break;
                    }
                }
                
                break;
            }
                
            case GameEvent::Events_SVSpawnMonster:
            {
                auto gs_spawn = static_cast<const GameEvent::SVSpawnMonster*>(gs_event->event());
                
                auto monster = Monster::create("res/monster.png");
                monster->SetUID(gs_spawn->monster_uid());
                monster->SetHealth(gs_spawn->hp());
                
                cocos2d::Vec2 log_coords(gs_spawn->x(),
                                         gs_spawn->y());
                cocos2d::Vec2 spritePos = LOG_TO_PHYS_COORD(log_coords,
                                                            monster->getContentSize());
                
                monster->SetLogicalPosition(log_coords);
                monster->setPosition(spritePos);
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
                
//                CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("res/step.mp3");
                if(gs_mov->target_type() == GameEvent::ActionMoveTarget_PLAYER)
                {
                    for(auto player : m_aPlayers)
                    {
                        if(player->GetUID() == gs_mov->target_uid())
                        {
                            auto new_pos = cocos2d::Vec2(gs_mov->x(), gs_mov->y());
                            
                            if(player->GetLogicalPosition().x > new_pos.x)
                            {
                                player->setTexture("res/player_left.png");
                            }
                            else if(player->GetLogicalPosition().x < new_pos.x)
                            {
                                player->setTexture("res/player_right.png");
                            }
                            else if(player->GetLogicalPosition().y < new_pos.y)
                            {
                                player->setTexture("res/player_up.png");
                            }
                            else if(player->GetLogicalPosition().y > new_pos.y)
                            {
                                player->setTexture("res/player_down.png");
                            }
                            
                            player->SetLogicalPosition(new_pos);
                            player->setPosition(LOG_TO_PHYS_COORD(new_pos,
                                                                  player->getContentSize()));
                            
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
                            
                            monster->SetLogicalPosition(new_pos);
                            monster->setPosition(LOG_TO_PHYS_COORD(new_pos,
                                                                   monster->getContentSize()));
                            
                            break;
                        }
                    }
                }
                
                break;
            }
                
            case GameEvent::Events_SVActionItem:
            {
                auto gs_item = static_cast<const GameEvent::SVActionItem*>(gs_event->event());
                auto item = std::find_if(m_aItems.begin(),
                                         m_aItems.end(),
                                         [gs_item](Item * item)
                                         {
                                             return item->GetUID() == gs_item->item_uid();
                                         });
                auto player = GetPlayerByUID(gs_item->player_uid());
                
                switch(gs_item->act_type())
                {
                    case GameEvent::ActionItemType_TAKE:
                    {
                        (*item)->setVisible(false);
                        (*item)->SetCarrierID(gs_item->player_uid());
                        player->GetInventory().push_back((*item)->GetUID());
                        
                        break;
                    }
                        
                    case GameEvent::ActionItemType_DROP:
                    {
                        (*item)->setVisible(true);
                        (*item)->SetCarrierID(0);
                        (*item)->SetLogicalPosition(player->GetLogicalPosition());
                        (*item)->setPosition(
                                             LOG_TO_PHYS_COORD(player->GetLogicalPosition(),
                                                               (*item)->getContentSize()));
                        
                            // delete item from players inventory
                        for(auto it = player->GetInventory().begin();
                            it != player->GetInventory().end();
                            ++it)
                        {
                            if((*it) == (*item)->GetUID())
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
                
            case GameEvent::Events_SVActionSwamp:
            {
                auto gs_swamp = static_cast<const GameEvent::SVActionSwamp*>(gs_event->event());
                
                switch(gs_swamp->status())
                {
                    case GameEvent::ActionSwampStatus_STARTED:
                    {
                        for(auto player : m_aPlayers)
                        {
                            if(player->GetUID() == gs_swamp->player_uid())
                            {
                                player->SetState(Player::State::SWAMP);
                                
                                if(player->GetUID() == m_pLocalPlayer->GetUID())
                                {
                                    m_eState = State::DROWNING;
                                    m_pSwampCombo->Show(true);
                                }
                                break;
                            }
                        }
                        
                        break;
                    }
                        
                    case GameEvent::ActionSwampStatus_DIED:
                    {
                        for(auto player : m_aPlayers)
                        {
                            if(player->GetUID() == gs_swamp->player_uid())
                            {
                                player->SetState(Player::State::DEAD);
                                
                                if(player->GetUID() == gs_swamp->player_uid())
                                {
                                    m_pSwampCombo->Show(false);
                                    m_pSwampCombo->Reset();
                                }
                                break;
                            }
                        }
                        
                        break;
                    }
                        
                    case GameEvent::ActionSwampStatus_ESCAPED:
                    {
                        for(auto player : m_aPlayers)
                        {
                            if(player->GetUID() == gs_swamp->player_uid())
                            {
                                player->SetState(Player::State::WALKING);
                                
                                if(player->GetUID() == m_pLocalPlayer->GetUID())
                                {
                                    m_pSwampCombo->Show(false);
                                    m_pSwampCombo->Reset();
                                    
                                    m_eState = State::WALKING;
                                }
                                
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
                            // p v p
                        if(sv_duel->target2_type() == GameEvent::ActionDuelTarget_PLAYER)
                        {
                            auto player1 = GetPlayerByUID(sv_duel->target1_uid());
                            auto player2 = GetPlayerByUID(sv_duel->target2_uid());
                            
                            player1->SetState(Player::State::DUEL_PLAYER);
                            player1->SetDuelTargetID(sv_duel->target2_uid());
                            
                            if(m_pLocalPlayer->GetUID() == player1->GetUID())
                            {
//                                CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("res/duel_start.mp3");
                                m_pDuelMode->Show(true);
                            }
                            
                            player2->SetState(Player::State::DUEL_PLAYER);
                            player2->SetDuelTargetID(sv_duel->target1_uid());
                            
                            if(m_pLocalPlayer->GetUID() == player2->GetUID())
                            {
//                                CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("res/duel_start.mp3");
                                m_pDuelMode->Show(true);
                            }
                        }
                            // p v m
                        else if(sv_duel->target2_type() == GameEvent::ActionDuelTarget_MONSTER)
                        {
                            auto player1 = GetPlayerByUID(sv_duel->target1_uid());
                            auto monster = GetMonsterByUID(sv_duel->target2_uid());
                            
                            player1->SetState(Player::State::DUEL_MONSTER);
                            monster->SetState(Monster::State::DUEL);
                            
                            if(m_pLocalPlayer->GetUID() == player1->GetUID())
                            {
//                                CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("res/duel_start.mp3");
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
                                // p v p
                            if(sv_duel->target2_type() == GameEvent::ActionDuelTarget_PLAYER)
                            {
                                auto player1 = GetPlayerByUID(sv_duel->target1_uid());
                                auto player2 = GetPlayerByUID(sv_duel->target2_uid());
                                
                                player2->SetHealth(player2->GetHealth()-1);
                            }
                                // p v m
                            else if(sv_duel->target2_type() == GameEvent::ActionDuelTarget_MONSTER)
                            {
                                auto player1 = GetPlayerByUID(sv_duel->target1_uid());
                                auto monster = GetMonsterByUID(sv_duel->target2_uid());
                                
                                    // TODO: Set monsters health
                                monster->SetHealth(monster->GetHealth()-1);
                            }
                        }
                            // monster attacks
                        else if(sv_duel->target1_type() == GameEvent::ActionDuelTarget_MONSTER)
                        {
                            auto monster = GetMonsterByUID(sv_duel->target1_uid());
                            auto player = GetPlayerByUID(sv_duel->target2_uid());
                            
                            player->SetHealth(player->GetHealth()-1);
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
                                
                                player1->SetState(Player::State::WALKING);
                                if(m_pLocalPlayer->GetUID() == player1->GetUID())
                                {
//                                    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("res/duel_win.mp3");
                                    m_pDuelMode->Show(false);
                                    m_pDuelMode->Reset();
                                }
                                
                                player2->SetState(Player::State::DEAD);
                                
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
                                
                                player->SetState(Player::State::WALKING);
                                if(m_pLocalPlayer->GetUID() == player->GetUID())
                                {
//                                    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("res/duel_win.mp3");
                                    m_pDuelMode->Show(false);
                                    m_pDuelMode->Reset();
                                }
                                
                                monster->SetState(Monster::State::DEAD);
                                
                            }
                        }
                            // monster killed
                        else if(sv_duel->target1_type() == GameEvent::ActionDuelTarget_MONSTER)
                        {
                            auto monster = GetMonsterByUID(sv_duel->target1_uid());
                            auto player = GetPlayerByUID(sv_duel->target2_uid());
                            
                            monster->SetState(Monster::State::WAITING);
                            
                            player->SetState(Player::State::DEAD);
                            
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
                        player->SetState(Player::State::WALKING);
                        
                        cocos2d::Vec2 log_coords(gs_resp->x(),
                                                 gs_resp->y());
                        cocos2d::Vec2 spritePos = LOG_TO_PHYS_COORD(log_coords,
                                                                    player->getContentSize());
                        player->SetLogicalPosition(log_coords);
                        player->SetHealth(gs_resp->hp());
                        player->SetMaxHealth(gs_resp->max_hp());
                        
                        player->setPosition(spritePos);
                        break;
                    }
                }
                
                break;
            }
                
            case GameEvent::Events_SVGameEnd:
            {
                auto gs_go = static_cast<const GameEvent::SVGameEnd*>(gs_event->event());
                if(gs_go->player_uid() == m_pLocalPlayer->GetUID())
                {
                    auto audio = CocosDenshion::SimpleAudioEngine::getInstance();
                    if(!audio->isBackgroundMusicPlaying())
                    {
//                        audio->playBackgroundMusic("res/win.mp3", false);
                    }
                }
                
                this->release();
                Director::getInstance()->replaceScene(MainMenuScene::createScene());
                
                break;
            }
                
            default:
                assert(false);
                break;
        }
    }
    
//        // update map
//    auto pl_pos = m_pLocalPlayer->GetPosition();
//    for(auto i = 0; i < m_oGameMap.GetMatrix().size(); ++i)
//    {
//        for(auto j = 0; j < m_oGameMap[i].size(); ++j)
//        {
//            if(m_oGameMap[i][j]->IsDiscovered())
//            {
//                m_oGameMap[i][j]->GetSprite()->setOpacity(100);
//            }
//            else
//            {
//                m_oGameMap[i][j]->GetSprite()->setOpacity(0);
//            }
//        }
//    }
//    
//    for(auto i = pl_pos.x-1; i <= pl_pos.x+1; ++i)
//    {
//        for(auto j = pl_pos.y-1; j <= pl_pos.y+1; ++j)
//        {
//            m_oGameMap[i][j]->SetDiscovered(true);
//        }
//    }
    
        // representation is based on state
    if(m_pLocalPlayer->GetState() == Player::State::WALKING)
    {
        m_pGameHUD->m_pState->setString("Just walking around...");
            // check that player can take some item
        for(auto& item : m_aItems)
        {
            if(item->isVisible() &&
               item->GetLogicalPosition() == m_pLocalPlayer->GetLogicalPosition())
            {
                m_pGameHUD->m_pTakeItem->setVisible(true);
                break;
            }
            m_pGameHUD->m_pTakeItem->setVisible(false);
        }
        
        auto pCam = Camera::getDefaultCamera();
        pCam->setPosition3D(Vec3(
                                 m_pLocalPlayer->getPosition().x,
                                 m_pLocalPlayer->getPosition().y,
                                 800));
    }
    else if(m_pLocalPlayer->GetState() == Player::State::SWAMP)
    {
        m_pGameHUD->m_pState->setString("You are drowning!\nEnter combo!!!");
        
        if(m_pSwampCombo->ComboDone())
        {
            m_pSwampCombo->Reset();
            
            auto cl_escape = GameEvent::CreateCLActionSwamp(builder,
                                                            m_pLocalPlayer->GetUID(),
                                                            GameEvent::ActionSwampStatus_ESCAPED);
            auto gs_event = GameEvent::CreateEvent(builder,
                                                   GameEvent::Events_CLActionSwamp,
                                                   cl_escape.Union());
            builder.Finish(gs_event);
            SendEventAndClear();
        }
    }
    else if(m_pLocalPlayer->GetState() == Player::State::DUEL_PLAYER)
    {
        m_pGameHUD->m_pState->setString("DUEL MODE! (PLAYER)");
        
        if(m_pDuelMode->ComboDone())
        {
            m_pDuelMode->Reset();
            
            auto cl_attack = GameEvent::CreateCLActionDuel(builder,
                                                           m_pLocalPlayer->GetUID(),
                                                           GameEvent::ActionDuelTarget_PLAYER,
                                                           m_pLocalPlayer->GetDuelTargetID(),
                                                           GameEvent::ActionDuelTarget_PLAYER,
                                                           GameEvent::ActionDuelType_ATTACK);
            auto sv_event = GameEvent::CreateEvent(builder,
                                                   GameEvent::Events_CLActionDuel,
                                                   cl_attack.Union());
            builder.Finish(sv_event);
            SendEventAndClear();
        }
        
        auto enemy = GetPlayerByUID(m_pLocalPlayer->GetDuelTargetID());
        m_pDuelMode->SetEnemyInfo(enemy->GetNickname(),
                                  enemy->GetHealth());
    }
    else if(m_pLocalPlayer->GetState() == Player::State::DUEL_MONSTER)
    {
        m_pGameHUD->m_pState->setString("DUEL MODE! (MONSTER)");
        
        if(m_pDuelMode->ComboDone())
        {
            m_pDuelMode->Reset();
            
            auto cl_attack = GameEvent::CreateCLActionDuel(builder,
                                                           m_pLocalPlayer->GetUID(),
                                                           GameEvent::ActionDuelTarget_PLAYER,
                                                           m_pLocalPlayer->GetDuelTargetID(),
                                                           GameEvent::ActionDuelTarget_MONSTER,
                                                           GameEvent::ActionDuelType_ATTACK);
            auto sv_event = GameEvent::CreateEvent(builder,
                                                   GameEvent::Events_CLActionDuel,
                                                   cl_attack.Union());
            builder.Finish(sv_event);
            SendEventAndClear();
        }
        
        auto enemy = GetMonsterByUID(m_pLocalPlayer->GetDuelTargetID());
        m_pDuelMode->SetEnemyInfo("Mike Wazowski",
                                  enemy->GetHealth());
    }
    else if(m_pLocalPlayer->GetState() == Player::State::DEAD)
    {
        m_pGameHUD->m_pState->setString("You are DEAD\nWait respawn");
    }
        // update hud
    m_pGameHUD->m_pHP->setString(StringUtils::format("Health: %d/%d",
                                                     m_pLocalPlayer->GetHealth(),
                                                     m_pLocalPlayer->GetMaxHealth()));
    std::string inventory = "Inventory:\n";
    for(auto item_id : m_pLocalPlayer->GetInventory())
    {
        Item * item = GetItemByUID(item_id);
        if(item->GetType() == Item::Type::KEY)
            inventory += "Key\n";
        else if(item->GetType() == Item::Type::SWORD)
            inventory += "Sword\n";
    }
    m_pGameHUD->m_pInventory->setString(inventory);
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

Player *
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
GameScene::SendEventAndClear()
{
    NetSystem::Instance().Socket().sendBytes(builder.GetBufferPointer(),
                                             builder.GetSize());
    builder.Clear();
}
