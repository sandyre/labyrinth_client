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
    
    m_eStatus = Status::REQUESTING_LOBBY;
    
    auto visibleSize = Director::getInstance()->getVisibleSize();
    
    m_pStatusInfo = Label::createWithTTF("STATUS: REQUESTING LOBBY",
                                         "fonts/ethnocentric rg.ttf", 24);
    m_pStatusInfo->setPosition(visibleSize.width / 2, visibleSize.height / 2);
    this->addChild(m_pStatusInfo);
    
    m_pLobbyInfo = Label::createWithTTF("Players:\n",
                                        "fonts/ethnocentric rg.ttf",
                                        15);
    m_pLobbyInfo->setPosition(visibleSize.width * 0.3, visibleSize.height * 0.9);
    this->addChild(m_pLobbyInfo);
    
    this->scheduleUpdate();
    return true;
}

void
PreGameScene::update(float delta)
{
    flatbuffers::FlatBufferBuilder builder;
    unsigned char buf[256];
    
    if(m_eStatus == Status::REQUESTING_LOBBY)
    {
        auto& socket = NetSystem::Instance().Socket();
        auto req_lobby = MSNet::CreateCLFindGame(builder, PlayerInfo::Instance().GetUID());
        auto ms_event = MSNet::CreateMSEvent(builder, MSNet::MSEvents_CLFindGame, req_lobby.Union());
        builder.Finish(ms_event);
        
        socket.sendBytes(builder.GetBufferPointer(),
                         builder.GetSize());
        builder.Clear();
        
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
    }
    else if(m_eStatus == Status::CONNECTING_TO_GS)
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
    }
    else if(m_eStatus == Status::WAITING_ACCEPT)
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
    }
    else if(m_eStatus == Status::WAITING_OTHERS)
    {
        auto& socket = NetSystem::Instance().Socket();
        while(socket.available())
        {
            socket.receiveBytes(buf, 256);
            
            auto gs_event = GameEvent::GetEvent(buf);
            if(gs_event->event_type() == GameEvent::Events_SVPlayerConnected)
            {
                auto& players_list = m_pGameScene->GetPlayersList();
                auto con_info = static_cast<const GameEvent::SVPlayerConnected*>(gs_event->event());
                
                auto iter = std::find_if(players_list.begin(),
                                         players_list.end(),
                                         [con_info](auto& pl)
                                         {
                                             return pl->GetUID() == con_info->player_uid();
                                         });
                
                    // player is not in-game already, add him
                if(iter == players_list.end())
                {
                    auto new_player = Player::create("res/player_down.png");
                    new_player->retain();
                    new_player->SetUID(con_info->player_uid());
                    new_player->SetNickname(con_info->nickname()->c_str()); // FIXME: no checks that its zero-terminated
                    players_list.push_back(new_player);
                }
            }
            else if(gs_event->event_type() == GameEvent::Events_SVGenerateMap)
            {
                auto& socket = NetSystem::Instance().Socket();
                auto gen_info = static_cast<const GameEvent::SVGenerateMap*>(gs_event->event());
                GameMap::Configuration sets;
                sets.nMapSize = gen_info->map_w();
                sets.nRoomSize = gen_info->room_w();
                sets.nSeed = gen_info->seed();
                
                m_pGameScene->GenerateMap(sets);
//                ResourceManager::Instance().PreloadSounds();
                
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
                m_pStatusInfo->setString("STATUS: LEVEL GENERATED, WAITING OTHERS");
            }
        }
        
        std::string lobbyinfo = "Server port [" + to_string(m_stGSAddr.port());
        lobbyinfo += "]\nPlayers list:\n";
        for(auto& player : m_pGameScene->GetPlayersList())
        {
            lobbyinfo += player->GetNickname();
            lobbyinfo += " [UID";
            lobbyinfo += to_string(player->GetUID());
            lobbyinfo += "]\n";
        }
        m_pLobbyInfo->setString(lobbyinfo);
    }
    else if(m_eStatus == Status::WAITING_SERVER_START)
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
    }
}
