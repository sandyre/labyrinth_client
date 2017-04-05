    //
    //  pregamescene.cpp
    //  labyrinth
    //
    //  Created by Aleksandr Borzikh on 07.02.17.
    //
    //

#include "pregamescene.hpp"

#include <sstream>
#include "accountinfo.hpp"
#include "gamescene.hpp"
#include <netsystem.hpp>
#include "msnet_generated.h"
#include "gsnet_generated.h"
#include "resourcemanager.hpp"

#include "gamelogic/units/units_inc.hpp"

template < typename T > std::string to_string( const T& n )
{
    std::ostringstream stm ;
    stm << n ;
    return stm.str() ;
}

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
    
    auto visibleSize = Director::getInstance()->getVisibleSize();

    m_pUI = new UIPregameScene;
    this->addChild(m_pUI);
    
    this->scheduleUpdate();
    return true;
}

void
PreGameScene::update(float delta)
{
    flatbuffers::FlatBufferBuilder builder;
    unsigned char buf[256];
    
    switch(m_eStatus)
    {
        case CONNECTING_TO_MS:
        {
            auto& socket = NetSystem::Instance().GetChannel(0);
            auto req_lobby = MSNet::CreateCLFindGame(builder,
                                                     AccountInfo::Instance().GetUID(),
                                                     GAMEVERSION_MAJOR,
                                                     GAMEVERSION_MINOR,
                                                     GAMEVERSION_BUILD);
            auto ms_event = MSNet::CreateMSEvent(builder,
                                                 MSNet::MSEvents_CLFindGame,
                                                 req_lobby.Union());
            builder.Finish(ms_event);
            
            socket.SendBytes(builder.GetBufferPointer(),
                             builder.GetSize());
            builder.Clear();
            
            socket.ReceiveBytes();
            
            if(socket.GetState() == NetSystem::ChannelState::DR_TIMEOUT)
            {
                MessageBox("Connection timeout", "Server unavailable");
                Director::getInstance()->popScene();
            }
            
            auto event = MSNet::GetMSEvent(socket.GetBuffer().data());
            if(event->event_type() == MSNet::MSEvents_SVFindGame)
            {
                auto con_resp = static_cast<const MSNet::SVFindGame*>(event->event());
                
                if(con_resp->response() == MSNet::ConnectionResponse_ACCEPTED)
                {
                    m_eStatus = REQUESTING_LOBBY;
                }
                else
                {
                    m_pUI->m_pStatusText->setString("SERVER REFUSED CONNECTION:\nINCOMPATIBLE VERSIONS");
                }
            }
            break;
        }
        case REQUESTING_LOBBY:
        {
            auto& socket = NetSystem::Instance().GetChannel(0);
            
            socket.ReceiveBytes();
            
            auto lobby_info = MSNet::GetMSEvent(socket.GetBuffer().data());
            if(lobby_info->event_type() == MSNet::MSEvents_MSGameFound)
            {
                auto gs_info = static_cast<const MSNet::MSGameFound*>(lobby_info->event());
                
                m_stGSAddr = Poco::Net::SocketAddress(socket.GetAddress().host(),
                                                      gs_info->gs_port());
                NetSystem::Instance().GetChannel(1).SetAddress(m_stGSAddr);
                
                m_eStatus = Status::CONNECTING_TO_GS;
                m_pUI->m_pStatusText->setString("STATUS: CONNECTING TO LOBBY");
            }
            break;
        }
        case CONNECTING_TO_GS:
        {
            auto& socket = NetSystem::Instance().GetChannel(1);
            auto nickname = builder.CreateString(AccountInfo::Instance().GetNickname());
            auto con_info = GameEvent::CreateCLConnection(builder,
                                                          AccountInfo::Instance().GetUID(),
                                                          nickname);
            auto gs_event = GameEvent::CreateMessage(builder,
                                                     GameEvent::Events_CLConnection,
                                                     con_info.Union());
            builder.Finish(gs_event);
            
            socket.SendBytes(builder.GetBufferPointer(),
                             builder.GetSize());
            builder.Clear();
            
            m_pUI->m_pStatusText->setString("STATUS: WAITING SERVER ACCEPTANCE");
            m_eStatus = Status::WAITING_ACCEPT;
            break;
        }
        case WAITING_ACCEPT:
        {
            auto& socket = NetSystem::Instance().GetChannel(1);
            char buf[256];
            socket.ReceiveBytes();
            
            auto accept = GameEvent::GetMessage(socket.GetBuffer().data());
            
            if(accept->event_type() == GameEvent::Events_SVConnectionStatus)
            {
                    // TODO: add refuse possibility
                m_eStatus = Status::WAITING_OTHERS;
                m_pUI->m_pStatusText->setString("STATUS: WAITING OTHER PLAYERS");
//                m_pPlayersList->setVisible(true);
            }
            break;
        }
        case WAITING_OTHERS:
        {
            auto& socket = NetSystem::Instance().GetChannel(1);
            while(socket.DataAvailable())
            {
                socket.ReceiveBytes();
                
                auto gs_event = GameEvent::GetMessage(socket.GetBuffer().data());
                if(gs_event->event_type() == GameEvent::Events_SVPlayerConnected)
                {
                    auto con_info = static_cast<const GameEvent::SVPlayerConnected*>(gs_event->event());
                    
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
                        m_aLobbyPlayers.push_back(player_info);
                        
                        m_pUI->m_pPlayersList->AddPlayer(player_info);
                    }
                }
                else if(gs_event->event_type() == GameEvent::Events_SVHeroPickStage)
                {
                    m_eStatus = Status::HERO_PICK_STAGE;
                    m_pUI->m_pStatusText->setString("STATUS: HERO PICK");
                }
            }
            
            break;
        }
        case HERO_PICK_STAGE:
        {
            auto& socket = NetSystem::Instance().GetChannel(1);
            while(socket.DataAvailable())
            {
                socket.ReceiveBytes();
                
                auto gs_event = GameEvent::GetMessage(socket.GetBuffer().data());
                if(gs_event->event_type() == GameEvent::Events_SVHeroPick)
                {
                    auto hero_pick = static_cast<const GameEvent::SVHeroPick*>(gs_event->event());
                    auto iter = std::find_if(m_aLobbyPlayers.begin(),
                                             m_aLobbyPlayers.end(),
                                             [hero_pick](auto& pl)
                                             {
                                                 return pl.nUID == hero_pick->player_uid();
                                             });
                    
                    iter->nHeroIndex = hero_pick->hero_type();
                    
                    if(iter->nUID == AccountInfo::Instance().GetUID())
                    {
//                        m_pReadyButton->setVisible(true);
                    }
                }
                else if(gs_event->event_type() == GameEvent::Events_SVGenerateMap)
                {
                    auto gen_info = static_cast<const GameEvent::SVGenerateMap*>(gs_event->event());
                    GameMap::Configuration sets;
                    sets.nMapSize = gen_info->map_w();
                    sets.nRoomSize = gen_info->room_w();
                    sets.nSeed = gen_info->seed();
                    
                    m_stMapConfig = sets;
                    
                    m_eStatus = Status::GENERATING_LEVEL;
                    m_pUI->m_pStatusText->setString("STATUS: GENERATING WORLD");
                }
            }
            break;
        }
        case GENERATING_LEVEL:
        {
            auto& socket = NetSystem::Instance().GetChannel(1);
            m_pGameScene->GenerateMap(m_stMapConfig);
            
            for(auto& player : m_aLobbyPlayers)
            {
                if(player.nHeroIndex == Hero::Type::AIR_ELEMENTALIST)
                {
                    auto air = AirElementalist::create("res/units/player_down.png");
                    air->SetUID(player.nUID);
                    air->SetName(player.sNickname);
                    air->retain();
                    m_pGameScene->GetPlayersList().push_back(air);
                }
                else if(player.nHeroIndex == Hero::Type::EARTH_ELEMENTALIST)
                {
                    auto earth = EarthElementalist::create("res/units/player_down.png");
                    earth->SetUID(player.nUID);
                    earth->SetName(player.sNickname);
                    earth->retain();
                    m_pGameScene->GetPlayersList().push_back(earth);
                }
                else if(player.nHeroIndex == Hero::Type::FIRE_ELEMENTALIST)
                {
                    auto fire = FireElementalist::create("res/units/player_down.png");
                    fire->SetUID(player.nUID);
                    fire->SetName(player.sNickname);
                    fire->retain();
                    m_pGameScene->GetPlayersList().push_back(fire);
                }
                else if(player.nHeroIndex == Hero::Type::WATER_ELEMENTALIST)
                {
                    auto water = WaterElementalist::create("res/units/player_down.png");
                    water->SetUID(player.nUID);
                    water->SetName(player.sNickname);
                    water->retain();
                    m_pGameScene->GetPlayersList().push_back(water);
                }
            }
            
                // notify server that generating is done
            auto gen_ok = GameEvent::CreateCLMapGenerated(builder,
                                                          AccountInfo::Instance().GetUID());
            auto cl_event = GameEvent::CreateMessage(builder,
                                                     GameEvent::Events_CLMapGenerated,
                                                     gen_ok.Union());
            builder.Finish(cl_event);
            
            socket.SendBytes(builder.GetBufferPointer(),
                             builder.GetSize());
            builder.Clear();
            
            m_eStatus = Status::WAITING_SERVER_START;
            m_pUI->m_pStatusText->setString("STATUS: WORLD GENERATED, WAITING OTHERS");
            break;
        }
        case WAITING_SERVER_START:
        {
            auto& socket = NetSystem::Instance().GetChannel(1);
            if(socket.DataAvailable())
            {
                socket.ReceiveBytes();
                
                auto gs_event = GameEvent::GetMessage(socket.GetBuffer().data());
                if(gs_event->event_type() == GameEvent::Events_SVGameStart)
                {
                    if(m_pGameScene->init())
                    {
                        auto scene = new Scene();
                        scene->addChild(m_pGameScene);
                        Director::getInstance()->replaceScene(scene);
                    }
                }
            }
            break;
        }
    }
}
