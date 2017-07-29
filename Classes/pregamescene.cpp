//
//  pregamescene.cpp
//  labyrinth
//
//  Created by Aleksandr Borzikh on 07.02.17.
//
//

#include "pregamescene.hpp"

#include "gameconfig.hpp"
#include "gamelogic/units/units_inc.hpp"
#include "gsnet_generated.h"
#include "netsystem.hpp"
#include "resources.hpp"
#include "resourcemanager.hpp"
#include "ui/ui_pregamescene.hpp"
#include "toolkit/SafePacketGetter.hpp"

#include <future>
#include <sstream>

template < typename T > std::string to_string( const T& n )
{
    std::ostringstream stm ;
    stm << n ;
    return stm.str() ;
}

using namespace GameMessage;
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
    auto uuid = builder.CreateString(GameConfiguration::Instance().GetUUID());
    auto nickname = builder.CreateString(GameConfiguration::Instance().GetPlayerName());
    auto con_info = GameMessage::CreateCLConnection(builder,
                                                    0,
                                                    nickname);
    auto gs_event = GameMessage::CreateMessage(builder,
                                               uuid,
                                               GameMessage::Messages_CLConnection,
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
            auto& localPlayer = _sessionDescriptor.LocalPlayer;
            if(localPlayer.Hero != Hero::FIRST_HERO)
            {
                localPlayer.Hero--;
                m_pUI->m_pHeroPick->m_pHeroPageView->scrollToPage(localPlayer.Hero);
                
                flatbuffers::FlatBufferBuilder builder;

                auto uuid = builder.CreateString(GameConfiguration::Instance().GetUUID());
                auto heropick = GameMessage::CreateCLHeroPick(builder,
                                                              localPlayer.Uid,
                                                              (GameMessage::HeroType)localPlayer.Hero);
                auto msg = GameMessage::CreateMessage(builder,
                                                      uuid,
                                                      GameMessage::Messages_CLHeroPick,
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
            auto& localPlayer = _sessionDescriptor.LocalPlayer;
            if(localPlayer.Hero != Hero::LAST_HERO)
            {
                localPlayer.Hero++;
                m_pUI->m_pHeroPick->m_pHeroPageView->scrollToPage(localPlayer.Hero);
                
                flatbuffers::FlatBufferBuilder builder;

                auto uuid = builder.CreateString(GameConfiguration::Instance().GetUUID());
                auto heropick = GameMessage::CreateCLHeroPick(builder,
                                                              localPlayer.Uid,
                                                              (GameMessage::HeroType)localPlayer.Hero);
                auto msg = GameMessage::CreateMessage(builder,
                                                      uuid,
                                                      GameMessage::Messages_CLHeroPick,
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
        SafePacketGetter safeGetter(_channel->native_handler());
        auto packet = safeGetter.Get<GameMessage::Message>();

        if(!packet)
            return;

        auto message = GameMessage::GetMessage(packet->Data.data());

        switch(message->payload_type())
        {
        case Messages_SVConnectionStatus:
        {
            auto status = static_cast<const SVConnectionStatus*>(message->payload());
            
            if(status->status() == ConnectionStatus_ACCEPTED)
            {
                m_pUI->m_pStatusText->setString("WAITING OTHER PLAYERS");

                GameSessionDescriptor::Player player;
                player.Name = GameConfiguration::Instance().GetPlayerName();
                player.Hero = Hero::Type::FIRST_HERO;
                player.Uid = status->player_uid();
                _sessionDescriptor.LocalPlayer = player;
            }
            else
            {
                NetSystem::Instance().RemoveChannel("gameserver");
                MessageBox("Error", "Lobby is full, try searching again");
                Director::getInstance()->popScene();
            }
            
            break;
        }
                
        case Messages_SVPlayerConnected:
        {
            auto con_info = static_cast<const SVPlayerConnected*>(message->payload());
            
            auto iter = std::find_if(_sessionDescriptor.Players.begin(),
                                     _sessionDescriptor.Players.end(),
                                     [con_info](auto& pl)
                                     {
                                         return pl.Uid == con_info->player_uid();
                                     });
            
                // player is not in-game already, add him
            if(iter == _sessionDescriptor.Players.end())
            {
                GameSessionDescriptor::Player player_info;
                player_info.Uid = con_info->player_uid();
                player_info.Name = con_info->nickname()->c_str();
                player_info.Hero = Hero::Type::FIRST_HERO;
                _sessionDescriptor.Players.push_back(player_info);
                
                m_pUI->m_pPlayersList->AddPlayer(player_info);
            }
            
            break;
        }

        case Messages_SVPlayerDisconnected:
        {
            auto disconnect = static_cast<const SVPlayerDisconnected*>(message->payload());

            auto iter = std::find_if(_sessionDescriptor.Players.begin(),
                                     _sessionDescriptor.Players.end(),
                                     [disconnect](auto& pl)
                                     {
                                         return pl.Uid == disconnect->player_uid();
                                     });

            if(iter == _sessionDescriptor.Players.end())
                assert(false); // cheto ne tak, lol

            if(disconnect->player_uid() == _sessionDescriptor.LocalPlayer.Uid)
            {
                MessageBox("Disconnect", "You have been disconnected by server (reason: connection timeout)");
                cocos2d::Director::getInstance()->popScene();
            }

            break;
        }
                
        case Messages_SVHeroPickStage:
        {
            m_pUI->m_pHeroPick->m_pLeftChange->setEnabled(true);
            m_pUI->m_pHeroPick->m_pRightChange->setEnabled(true);
            m_pUI->m_pStatusText->setString("HERO PICK STAGE");

                // make checkbox selectable
            for(auto& player_inf : m_pUI->m_pPlayersList->m_aPlayers)
            {
                if(player_inf->m_stPlayerInfo.Uid == _sessionDescriptor.LocalPlayer.Uid)
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
                                  auto uuid = builder.CreateString(GameConfiguration::Instance().GetUUID());
                                  auto ready_msg = GameMessage::CreateCLReadyToStart(builder,
                                                                                     player_inf->m_stPlayerInfo.Uid);
                                  auto msg = GameMessage::CreateMessage(builder,
                                                                        uuid,
                                                                        GameMessage::Messages_CLReadyToStart,
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
                
        case Messages_SVHeroPick:
        {
            auto hero_pick = static_cast<const GameMessage::SVHeroPick*>(message->payload());
            auto iter = std::find_if(_sessionDescriptor.Players.begin(),
                                     _sessionDescriptor.Players.end(),
                                     [hero_pick](auto& pl)
                                     {
                                         return pl.Uid == hero_pick->player_uid();
                                     });
            
            iter->Hero = hero_pick->hero_type();
            
            for(auto& player_info : m_pUI->m_pPlayersList->m_aPlayers)
            {
                if(player_info->m_stPlayerInfo.Uid == hero_pick->player_uid())
                {
                    player_info->m_pHeroImage->loadTexture(HeroIcons[iter->Hero]);
                }
            }
            
            break;
        }
                
        case Messages_SVReadyToStart:
        {
            auto player_ready = static_cast<const GameMessage::SVReadyToStart*>(message->payload());
            
            for(auto& player_info : m_pUI->m_pPlayersList->m_aPlayers)
            {
                if(player_info->m_stPlayerInfo.Uid == player_ready->player_uid())
                {
                    player_info->m_pReadyStatus->setSelected(true);
                }
            }
            
            break;
        }
                
        case Messages_SVGenerateMap:
        {
            auto gen_info = static_cast<const GameMessage::SVGenerateMap*>(message->payload());
            GameMap::Configuration sets;
            sets.nMapSize = gen_info->map_w();
            sets.nRoomSize = gen_info->room_w();
            sets.nSeed = gen_info->seed();
            _sessionDescriptor.MapConf = sets;
            
            m_eStatus = Status::GENERATING_LEVEL;
            m_pUI->m_pStatusText->setString("GENERATING WORLD");
            
                // launch async'd level generation, which will AUTOMATICLY notify server that its done
            std::async(std::launch::async,
                       [this]()
                       {
                           m_pGameScene->InitWorld(_sessionDescriptor);

                               // notify server that generating is done
                           flatbuffers::FlatBufferBuilder builder;
                           auto uuid = builder.CreateString(GameConfiguration::Instance().GetUUID());
                           auto gen_ok = GameMessage::CreateCLMapGenerated(builder,
                                                                           _sessionDescriptor.LocalPlayer.Uid);
                           auto cl_event = GameMessage::CreateMessage(builder,
                                                                      uuid,
                                                                      GameMessage::Messages_CLMapGenerated,
                                                                      gen_ok.Union());
                           builder.Finish(cl_event);

                           _channel->PushPacket(std::vector<uint8_t>(builder.GetBufferPointer(),
                                                                     builder.GetBufferPointer() + builder.GetSize()));
                       });

            break;
        }
                
        case Messages_SVGameStart:
        {
            if(m_pGameScene->init())
            {
                auto scene = new Scene();
                scene->addChild(m_pGameScene);
                Director::getInstance()->replaceScene(scene);
            }
            
            break;
        }
                
        case Messages_SVPing:
        {
            flatbuffers::FlatBufferBuilder builder;
            auto uuid = builder.CreateString(GameConfiguration::Instance().GetUUID());
            auto ping = CreateCLPing(builder);
            auto msg = CreateMessage(builder,
                                     uuid,
                                     Messages_CLPing,
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
