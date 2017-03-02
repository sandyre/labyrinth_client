//
//  pregamescene.cpp
//  labyrinth
//
//  Created by Aleksandr Borzikh on 07.02.17.
//
//

#include "pregamescene.hpp"

#include <sstream>
#include "playerinfo.hpp"
#include "gamescene.hpp"
#include <netsystem.hpp>
#include "msnet_generated.h"
#include "gsnet_generated.h"
#include "resourcemanager.hpp"

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
    
    m_pStatusInfo = Label::createWithTTF("STATUS: CONNECTING TO NETWORK",
                                         "fonts/ethnocentric rg.ttf", 24);
    m_pStatusInfo->setPosition(visibleSize.width / 2, visibleSize.height / 2);
    this->addChild(m_pStatusInfo);
    
    m_pLobbyInfo = Label::createWithTTF("Players:\n",
                                        "fonts/ethnocentric rg.ttf",
                                        15);
    m_pLobbyInfo->setPosition(visibleSize.width * 0.3, visibleSize.height * 0.9);
    this->addChild(m_pLobbyInfo);
    
    auto eventListenerTouch = EventListenerTouchOneByOne::create();
    eventListenerTouch->onTouchBegan = [this](Touch * touch, Event * event)
    {
        if(m_pRogueSprite->getBoundingBox().containsPoint(touch->getLocation()))
        {
            m_pReadyButton->setVisible(true);
            
            flatbuffers::FlatBufferBuilder builder;
            auto sv_pick = GameEvent::CreateCLHeroPick(builder,
                                                       PlayerInfo::Instance().GetUID(),
                                                       GameEvent::Hero_ROGUE);
            auto sv_event = GameEvent::CreateEvent(builder,
                                                   GameEvent::Events_CLHeroPick,
                                                   sv_pick.Union());
            builder.Finish(sv_event);
            NetSystem::Instance().Socket().sendBytes(builder.GetBufferPointer(),
                                                     builder.GetSize());
            builder.Clear();
            
            return true;
        }
        if(m_pPaladinSprite->getBoundingBox().containsPoint(touch->getLocation()))
        {
            m_pReadyButton->setVisible(true);
            
            flatbuffers::FlatBufferBuilder builder;
            auto sv_pick = GameEvent::CreateCLHeroPick(builder,
                                                       PlayerInfo::Instance().GetUID(),
                                                       GameEvent::Hero_PALADIN);
            auto sv_event = GameEvent::CreateEvent(builder,
                                                   GameEvent::Events_CLHeroPick,
                                                   sv_pick.Union());
            builder.Finish(sv_event);
            NetSystem::Instance().Socket().sendBytes(builder.GetBufferPointer(),
                                                     builder.GetSize());
            builder.Clear();
            
            return true;
        }
        if(m_pReadyButton->getBoundingBox().containsPoint(touch->getLocation()))
        {
            flatbuffers::FlatBufferBuilder builder;
            auto sv_ready = GameEvent::CreateCLReadyToStart(builder,
                                                            PlayerInfo::Instance().GetUID());
            auto sv_event = GameEvent::CreateEvent(builder,
                                                   GameEvent::Events_CLReadyToStart,
                                                   sv_ready.Union());
            builder.Finish(sv_event);
            NetSystem::Instance().Socket().sendBytes(builder.GetBufferPointer(),
                                                     builder.GetSize());
            builder.Clear();
            
            return true;
        }
        
        return false;
    };
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(eventListenerTouch, this);
    
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
            auto& socket = NetSystem::Instance().Socket();
            auto req_lobby = MSNet::CreateCLFindGame(builder,
                                                     PlayerInfo::Instance().GetUID(),
                                                     GAMEVERSION_MAJOR,
                                                     GAMEVERSION_MINOR,
                                                     GAMEVERSION_BUILD);
            auto ms_event = MSNet::CreateMSEvent(builder,
                                                 MSNet::MSEvents_CLFindGame,
                                                 req_lobby.Union());
            builder.Finish(ms_event);
            
            socket.sendBytes(builder.GetBufferPointer(),
                             builder.GetSize());
            builder.Clear();
            
            socket.receiveBytes(buf, 256);
            
            auto event = MSNet::GetMSEvent(buf);
            if(event->event_type() == MSNet::MSEvents_SVFindGame)
            {
                auto con_resp = static_cast<const MSNet::SVFindGame*>(event->event());
                
                if(con_resp->response() == MSNet::ConnectionResponse_ACCEPTED)
                {
                    m_eStatus = REQUESTING_LOBBY;
                }
                else
                {
                    m_pStatusInfo->setString("SERVER REFUSED CONNECTION:\nINCOMPATIBLE VERSIONS");
                }
            }
            break;
        }
        case REQUESTING_LOBBY:
        {
            auto& socket = NetSystem::Instance().Socket();
            
            socket.receiveBytes(buf, 256);
            auto lobby_info = MSNet::GetMSEvent(buf);
            if(lobby_info->event_type() == MSNet::MSEvents_MSGameFound)
            {
                auto gs_info = static_cast<const MSNet::MSGameFound*>(lobby_info->event());
                
                m_stGSAddr = Poco::Net::SocketAddress(socket.peerAddress().host(),
                                                      gs_info->gs_port());
                NetSystem::Instance().Socket().connect(m_stGSAddr);
                
                m_eStatus = Status::CONNECTING_TO_GS;
                m_pStatusInfo->setString("STATUS: CONNECTING TO LOBBY");
            }
            break;
        }
        case CONNECTING_TO_GS:
        {
            auto& socket = NetSystem::Instance().Socket();
            auto nickname = builder.CreateString(PlayerInfo::Instance().GetNickname());
            auto con_info = GameEvent::CreateCLConnection(builder,
                                                          PlayerInfo::Instance().GetUID(),
                                                          nickname);
            auto gs_event = GameEvent::CreateEvent(builder,
                                                   GameEvent::Events_CLConnection,
                                                   con_info.Union());
            builder.Finish(gs_event);
            
            socket.sendBytes(builder.GetBufferPointer(),
                             builder.GetSize());
            builder.Clear();
            
            m_pStatusInfo->setString("STATUS: WAITING SERVER ACCEPTANCE");
            m_eStatus = Status::WAITING_ACCEPT;
            break;
        }
        case WAITING_ACCEPT:
        {
            auto& socket = NetSystem::Instance().Socket();
            char buf[256];
            socket.receiveBytes(buf, 256);
            auto accept = GameEvent::GetEvent(buf);
            
            if(accept->event_type() == GameEvent::Events_SVConnectionStatus)
            {
                    // TODO: add refuse possibility
                m_eStatus = Status::WAITING_OTHERS;
                m_pStatusInfo->setString("STATUS: WAITING OTHER PLAYERS");
                m_pLobbyInfo->setVisible(true);
            }
            break;
        }
        case WAITING_OTHERS:
        {
            auto& socket = NetSystem::Instance().Socket();
            while(socket.available())
            {
                socket.receiveBytes(buf, 256);
                
                auto gs_event = GameEvent::GetEvent(buf);
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
                        PlayerConnectionInfo player_info;
                        player_info.nUID = con_info->player_uid();
                        player_info.sNickname = con_info->nickname()->c_str();
                        m_aLobbyPlayers.push_back(player_info);
                    }
                }
                else if(gs_event->event_type() == GameEvent::Events_SVHeroPickStage)
                {
                    m_eStatus = Status::HERO_PICK_STAGE;
                    auto size = Director::getInstance()->getVisibleSize();
                    m_pStatusInfo->setPosition(cocos2d::Vec2(size.width / 2,
                                                             size.height * 0.8));
                    m_pReadyButton = Sprite::create("res/ready_button.png");
                    m_pReadyButton->setPosition(cocos2d::Vec2(size.width / 2,
                                                              size.height * 0.3));
                    m_pReadyButton->setVisible(false);
                    this->addChild(m_pReadyButton);
                    m_pStatusInfo->setString("STATUS: HERO PICKING");
                    
                    m_pRogueSprite = Sprite::create("res/rogue.png");
                    m_pRogueSprite->setScale(2.0);
                    m_pRogueSprite->setPosition(cocos2d::Vec2(size.width * 0.3,
                                                              size.height * 0.6));
                    this->addChild(m_pRogueSprite);
                    
                    m_pPaladinSprite = Sprite::create("res/paladin.png");
                    m_pPaladinSprite->setScale(2.0);
                    m_pPaladinSprite->setPosition(cocos2d::Vec2(size.width * 0.6,
                                                              size.height * 0.6));
                    this->addChild(m_pPaladinSprite);
                }
            }
            
            std::string lobbyinfo = "Server port [" + to_string(m_stGSAddr.port());
            lobbyinfo += "]\nPlayers list:\n";
            for(auto& player : m_aLobbyPlayers)
            {
                lobbyinfo += player.sNickname;
                lobbyinfo += " [UID";
                lobbyinfo += to_string(player.nUID);
                lobbyinfo += "]\n";
            }
            m_pLobbyInfo->setString(lobbyinfo);
            break;
        }
        case HERO_PICK_STAGE:
        {
            auto& socket = NetSystem::Instance().Socket();
            while(socket.available())
            {
                socket.receiveBytes(buf, 256);
                
                auto gs_event = GameEvent::GetEvent(buf);
                if(gs_event->event_type() == GameEvent::Events_SVHeroPick)
                {
                    auto hero_pick = static_cast<const GameEvent::SVHeroPick*>(gs_event->event());
                    auto iter = std::find_if(m_aLobbyPlayers.begin(),
                                             m_aLobbyPlayers.end(),
                                             [hero_pick](auto& pl)
                                             {
                                                 return pl.nUID == hero_pick->player_uid();
                                             });
                    
                    iter->eHeroPicked = (Hero)hero_pick->hero_type();
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
                    m_pStatusInfo->setString("STATUS: GENERATING WORLD");
                }
            }
            break;
        }
        case GENERATING_LEVEL:
        {
            auto& socket = NetSystem::Instance().Socket();
            m_pGameScene->GenerateMap(m_stMapConfig);
            
            for(auto& player : m_aLobbyPlayers)
            {
                if(player.eHeroPicked == Hero::ROGUE)
                {
                    auto rogue = Rogue::create("res/player_down.png");
                    rogue->SetUID(player.nUID);
                    rogue->SetNickname(player.sNickname);
                    rogue->retain();
                    m_pGameScene->GetPlayersList().push_back(rogue);
                }
                else if(player.eHeroPicked == Hero::PALADIN)
                {
                    auto paladin = Paladin::create("res/player_down.png");
                    paladin->SetUID(player.nUID);
                    paladin->SetNickname(player.sNickname);
                    paladin->retain();
                    m_pGameScene->GetPlayersList().push_back(paladin);
                }
            }
            
                // notify server that generating is done
            auto gen_ok = GameEvent::CreateCLMapGenerated(builder,
                                                          PlayerInfo::Instance().GetUID());
            auto cl_event = GameEvent::CreateEvent(builder,
                                                   GameEvent::Events_CLMapGenerated,
                                                   gen_ok.Union());
            builder.Finish(cl_event);
            
            socket.sendBytes(builder.GetBufferPointer(),
                             builder.GetSize());
            builder.Clear();
            
            m_eStatus = Status::WAITING_SERVER_START;
            m_pStatusInfo->setString("STATUS: WORLD GENERATED, WAITING OTHERS");
            break;
        }
        case WAITING_SERVER_START:
        {
            auto& socket = NetSystem::Instance().Socket();
            if(socket.available())
            {
                socket.receiveBytes(buf, 256);
                
                auto gs_event = GameEvent::GetEvent(buf);
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
