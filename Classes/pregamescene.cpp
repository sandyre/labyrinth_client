//
//  pregamescene.cpp
//  labyrinth
//
//  Created by Aleksandr Borzikh on 07.02.17.
//
//

#include "pregamescene.hpp"

#include "gameconfig.hpp"
#include "gamescene.hpp"
#include "gamelogic/units/units_inc.hpp"
#include "gsnet_generated.h"
#include "netsystem.hpp"
#include "resources.hpp"
#include "resourcemanager.hpp"

#include <future>
#include <sstream>

template < typename T > std::string to_string( const T& n )
{
    std::ostringstream stm ;
    stm << n ;
    return stm.str() ;
}

using namespace GameEvent;
USING_NS_CC;

Scene *
PreGameScene::createScene()
{
    auto scene = Scene::create();
    auto layer = PreGameScene::create();
    scene->addChild(layer);
    
    return scene;
}

bool
PreGameScene::init()
{
    if(!Layer::init())
    {
        return false;
    }
    
        // setup GS - we will build it
    m_pGameScene = new GameScene();
    m_pGameScene->retain();
    
    m_eStatus = Status::CONNECTING_TO_MS;
    
    m_pUI = new UIPregameScene;
    this->addChild(m_pUI);
    
    _channel = NetSystem::Instance().GetChannel("gameserver");
    flatbuffers::FlatBufferBuilder builder;
    auto nickname = builder.CreateString(GameConfiguraton::Instance().GetPlayerName());
    auto con_info = GameEvent::CreateCLConnection(builder,
                                                  GameConfiguraton::Instance().GetUID(),
                                                  nickname);
    auto gs_event = GameEvent::CreateMessage(builder,
                                             GameConfiguraton::Instance().GetUID(),
                                             GameEvent::Events_CLConnection,
                                             con_info.Union());
    builder.Finish(gs_event);
    _channel->PushPacket(std::vector<uint8_t>(builder.GetBufferPointer(),
                                              builder.GetBufferPointer() + builder.GetSize()));
    
        // add listeners to hero pick platform
    auto& left_hero_button = m_pUI->m_pHeroPick->m_pLeftChange;
    auto left_button_callback = [this](Ref * pSender, ui::Widget::TouchEventType type)
    {
        if(type == ui::Widget::TouchEventType::ENDED)
        {
            auto player = std::find_if(m_aLobbyPlayers.begin(),
                                       m_aLobbyPlayers.end(),
                                       [this](PlayerInfo player)
                                       {
                                           return player.nUID == GameConfiguraton::Instance().GetUID();
                                       });
            if(player->nHeroIndex != Hero::FIRST_HERO)
            {
                player->nHeroIndex--;
                m_pUI->m_pHeroPick->m_pHeroPageView->scrollToPage(player->nHeroIndex);
                
                flatbuffers::FlatBufferBuilder builder;
                
                auto heropick = GameEvent::CreateCLHeroPick(builder,
                                                            player->nUID,
                                                            (GameEvent::HeroType)player->nHeroIndex);
                auto msg = GameEvent::CreateMessage(builder,
                                                    GameConfiguraton::Instance().GetUID(),
                                                    GameEvent::Events_CLHeroPick,
                                                    heropick.Union());
                builder.Finish(msg);
                
                _channel->PushPacket(std::vector<uint8_t>(builder.GetBufferPointer(),
                                                          builder.GetBufferPointer() + builder.GetSize()));
            }
        }
    };
    left_hero_button->addTouchEventListener(left_button_callback);
    left_hero_button->setEnabled(false);
    
    auto& right_hero_button = m_pUI->m_pHeroPick->m_pRightChange;
    auto right_button_callback = [this](Ref * pSender, ui::Widget::TouchEventType type)
    {
        if(type == ui::Widget::TouchEventType::ENDED)
        {
            auto player = std::find_if(m_aLobbyPlayers.begin(),
                                       m_aLobbyPlayers.end(),
                                       [this](PlayerInfo player)
                                       {
                                           return player.nUID == GameConfiguraton::Instance().GetUID();
                                       });
            if(player->nHeroIndex != Hero::LAST_HERO)
            {
                player->nHeroIndex++;
                m_pUI->m_pHeroPick->m_pHeroPageView->scrollToPage(player->nHeroIndex);
                
                flatbuffers::FlatBufferBuilder builder;
                
                auto heropick = GameEvent::CreateCLHeroPick(builder,
                                                            player->nUID,
                                                            (GameEvent::HeroType)player->nHeroIndex);
                auto msg = GameEvent::CreateMessage(builder,
                                                    GameConfiguraton::Instance().GetUID(),
                                                    GameEvent::Events_CLHeroPick,
                                                    heropick.Union());
                builder.Finish(msg);
                
                _channel->PushPacket(std::vector<uint8_t>(builder.GetBufferPointer(),
                                                          builder.GetBufferPointer() + builder.GetSize()));
            }
        }
    };
    right_hero_button->addTouchEventListener(right_button_callback);
    right_hero_button->setEnabled(false);
    
    this->scheduleUpdate();
    return true;
}

void
PreGameScene::update(float delta)
{
    if(_channel->Available())
    {
        auto packet = _channel->PopPacket();
        
        auto msg = GetMessage(packet.data());
        
        switch (msg->event_type())
        {
        case Events_SVConnectionStatus:
        {
            auto status = static_cast<const SVConnectionStatus*>(msg->event());
            
            if(status->status() == ConnectionStatus_ACCEPTED)
            {
                m_pUI->m_pStatusText->setString("WAITING OTHER PLAYERS");
            }
            else
            {
                NetSystem::Instance().RemoveChannel("gameserver");
                MessageBox("Error", "Lobby is full, try searching again");
                Director::getInstance()->popScene();
            }
            
            break;
        }
                
        case Events_SVPlayerConnected:
        {
            auto con_info = static_cast<const SVPlayerConnected*>(msg->event());
            
            auto iter = std::find_if(m_aLobbyPlayers.begin(),
                                     m_aLobbyPlayers.end(),
                                     [con_info](auto& pl)
                                     {
                                         return pl.nUID == con_info->player_uid();
                                     });
            
                // player is not in-game already, add him
            if(iter == m_aLobbyPlayers.end())
            {
                PlayerInfo player_info;
                player_info.nUID = con_info->player_uid();
                player_info.sNickname = con_info->nickname()->c_str();
                player_info.nHeroIndex = 0;
                m_aLobbyPlayers.push_back(player_info);
                
                m_pUI->m_pPlayersList->AddPlayer(player_info);
            }
            
            break;
        }
                
        case Events_SVHeroPickStage:
        {
            m_pUI->m_pHeroPick->m_pLeftChange->setEnabled(true);
            m_pUI->m_pHeroPick->m_pRightChange->setEnabled(true);
            m_pUI->m_pStatusText->setString("HERO PICK STAGE");

                // make checkbox selectable
            for(auto& player_inf : m_pUI->m_pPlayersList->m_aPlayers)
            {
                if(player_inf->m_stPlayerInfo.nUID == GameConfiguraton::Instance().GetUID())
                {
                        // TODO: i dont know why, but it works that way
                        // that 2 event listeners are triggered
                    player_inf->m_pReadyStatus->setEnabled(true);
                    player_inf->m_pReadyStatus->addTouchEventListener(
                                                                      [this, player_inf](Ref * pSender, ui::Widget::TouchEventType type)
                                                                      {
                                                                          if(type == ui::Widget::TouchEventType::ENDED)
                                                                          {
                                                                              if(player_inf->m_pReadyStatus->isSelected())
                                                                              {
                                                                                  player_inf->m_pReadyStatus->setSelected(true);
                                                                                  player_inf->m_pReadyStatus->setTouchEnabled(false);

                                                                                  flatbuffers::FlatBufferBuilder builder;
                                                                                  auto ready_msg = GameEvent::CreateCLReadyToStart(builder,
                                                                                                                                   player_inf->m_stPlayerInfo.nUID);
                                                                                  auto msg = GameEvent::CreateMessage(builder,
                                                                                                                      GameConfiguraton::Instance().GetUID(),
                                                                                                                      GameEvent::Events_CLReadyToStart,
                                                                                                                      ready_msg.Union());
                                                                                  builder.Finish(msg);

                                                                                  _channel->PushPacket(std::vector<uint8_t>(builder.GetBufferPointer(),
                                                                                                                            builder.GetBufferPointer() + builder.GetSize()));
                                                                              }
                                                                          }
                                                                      });
                }
            }
            
            break;
        }
                
        case Events_SVHeroPick:
        {
            auto hero_pick = static_cast<const GameEvent::SVHeroPick*>(msg->event());
            auto iter = std::find_if(m_aLobbyPlayers.begin(),
                                     m_aLobbyPlayers.end(),
                                     [hero_pick](auto& pl)
                                     {
                                         return pl.nUID == hero_pick->player_uid();
                                     });
            
            iter->nHeroIndex = hero_pick->hero_type();
            
            for(auto& player_info : m_pUI->m_pPlayersList->m_aPlayers)
            {
                if(player_info->m_stPlayerInfo.nUID == hero_pick->player_uid())
                {
                    player_info->m_pHeroImage->loadTexture(HeroIcons[iter->nHeroIndex]);
                }
            }
            
            break;
        }
                
        case Events_SVReadyToStart:
        {
            auto player_ready = static_cast<const GameEvent::SVReadyToStart*>(msg->event());
            
            for(auto& player_info : m_pUI->m_pPlayersList->m_aPlayers)
            {
                if(player_info->m_stPlayerInfo.nUID == player_ready->player_uid())
                {
                    player_info->m_pReadyStatus->setSelected(true);
                }
            }
            
            break;
        }
                
        case Events_SVGenerateMap:
        {
            auto gen_info = static_cast<const GameEvent::SVGenerateMap*>(msg->event());
            GameMap::Configuration sets;
            sets.nMapSize = gen_info->map_w();
            sets.nRoomSize = gen_info->room_w();
            sets.nSeed = gen_info->seed();
            m_stMapConfig = sets;
            
            m_eStatus = Status::GENERATING_LEVEL;
            m_pUI->m_pStatusText->setString("GENERATING WORLD");
            
                // launch async'd level generation, which will AUTOMATICLY notify server that its done
            std::future<int> gen = std::async(std::launch::async, [this]()
                       {
                           m_pGameScene->GetGameWorld()->CreateGameMap(m_stMapConfig);
                           
                           for(auto& player : m_aLobbyPlayers)
                           {
                               m_pGameScene->GetGameWorld()->AddPlayer(player);
                           }
                           
                           flatbuffers::FlatBufferBuilder builder;
                               // notify server that generating is done
                           auto gen_ok = GameEvent::CreateCLMapGenerated(builder,
                                                                         GameConfiguraton::Instance().GetUID());
                           auto cl_event = GameEvent::CreateMessage(builder,
                                                                    GameConfiguraton::Instance().GetUID(),
                                                                    GameEvent::Events_CLMapGenerated,
                                                                    gen_ok.Union());
                           builder.Finish(cl_event);
                           
                           _channel->PushPacket(std::vector<uint8_t>(builder.GetBufferPointer(),
                                                                     builder.GetBufferPointer() + builder.GetSize()));
                           return 0;
                       });

            break;
        }
                
        case Events_SVGameStart:
        {
            if(m_pGameScene->init())
            {
                auto scene = new Scene();
                scene->addChild(m_pGameScene);
                Director::getInstance()->replaceScene(scene);
            }
            
            break;
        }
                
        case Events_SVPing:
        {
            flatbuffers::FlatBufferBuilder builder;
            auto ping = CreateCLPing(builder);
            auto msg = CreateMessage(builder,
                                     GameConfiguraton::Instance().GetUID(),
                                     Events_CLPing,
                                     ping.Union());
            builder.Finish(msg);
            _channel->PushPacket(std::vector<uint8_t>(builder.GetBufferPointer(),
                                                      builder.GetBufferPointer() + builder.GetSize()));
            
            break;
        }
            
        default:
            break;
        }
    }
}
