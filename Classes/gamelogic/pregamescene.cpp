//
//  pregamescene.cpp
//  labyrinth
//
//  Created by Aleksandr Borzikh on 04.02.17.
//
//

#include "pregamescene.hpp"
#include "netsystem.hpp"
#include "playerinfo.hpp"
#include "gamescene.hpp"
#include "gamescenesets.hpp"

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
    
    m_eState = PreGameScene::State::CONNECTING_TO_MS;
    
    auto visibleSize = Director::getInstance()->getVisibleSize();
    
    m_pStatusLabel = Label::createWithTTF("STATUS: CONNECTING TO MS",
                                          "fonts/arial.ttf",
                                          32);
    
    m_pStatusLabel->setPosition(visibleSize.width * 0.5,
                             visibleSize.height * 0.5);
    this->addChild(m_pStatusLabel);
    
    this->scheduleUpdate();
    return true;
}

void
PreGameScene::update(float delta)
{
//    if(m_eState == PreGameScene::State::CONNECTING_TO_MS)
//    {
//        auto& socket = NetSystem::Instance().Socket();
//        Poco::Net::SocketAddress msAddr("localhost", 1930);
//        socket.connect(msAddr);
//        
//            // request game
//        MSPacket req_game;
//        req_game.eType = MSPacket::Type::CL_FIND_GAME;
//        req_game.nUID = PlayerInfo::Instance().GetUID();
//        socket.sendBytes(&req_game, sizeof(req_game));
//        socket.receiveBytes(&req_game, sizeof(req_game));
//        
//            // game found, redirect socket to GS
//        MSPackets::MSGameFound * gs_addr = reinterpret_cast<MSPackets::MSGameFound*>(req_game.aData);
//        m_stGSAddr = Poco::Net::SocketAddress(msAddr.host(), gs_addr->nGSPort);
//        socket.connect(m_stGSAddr);
//        
//        m_pStatusLabel->setString(StringUtils::format("STATUS: CONNECTING TO GS"));
//        m_eState = PreGameScene::State::CONNECTING_TO_GS;
//    }
//    else if(m_eState == PreGameScene::State::CONNECTING_TO_GS)
//    {
//        auto& socket = NetSystem::Instance().Socket();
//        GamePacket pack;
//        new (&pack.mCLConnect) CLConnect;
//        pack.mCLConnect.nPlayerUID = PlayerInfo::Instance().GetUID();
//        strncpy(pack.mCLConnect.sNickname, PlayerInfo::Instance().GetNickname().c_str(), 16); // FIXME: possibly dangerous
//        socket.sendBytes(&pack, sizeof(pack));
//        
//        m_pStatusLabel->setString(StringUtils::format("STATUS: JOINING LOBBY"));
//        m_eState = PreGameScene::State::WAITING_PLAYERS;
//    }
//    else if(m_eState == PreGameScene::State::WAITING_PLAYERS)
//    {
//        GamePacket pack;
//        NetSystem::Instance().Socket().receiveBytes(&pack, sizeof(pack));
//        
//        if(pack.eType == GamePacket::Type::SRV_PLAYER_CONNECT)
//        {
//        }
//    }
//    else if(m_eState == PreGameScene::State::GENERATING_LEVEL)
//    {
//        using namespace GamePackets;
//        auto& socket = NetSystem::Instance().Socket();
//        GamePacket gen_map;
//        socket.receiveBytes(&gen_map, sizeof(gen_map));
//        SRVGenMap * gen_info = reinterpret_cast<SRVGenMap*>(gen_map.aData);
//        
//        GameMap::GameMapSettings sets;
//        sets.nChunks = gen_info->nChunkN;
//        sets.nSeed = gen_info->nSeed;
//        sets.nChunkWidth = 10;
//        sets.nChunkHeight = 10;
//        m_pGameWorld = new GameWorld();
////        m_pGameWorld->init_world();
//        m_pGameWorld->connect_to_server(m_stGSAddr);
//        
//        gen_map.eType = GamePacket::Type::CL_GEN_MAP_OK;
//        gen_map.nUID  = PlayerInfo::Instance().GetUID();
//        socket.sendBytes(&gen_map, sizeof(gen_map));
//        
//        m_pStatusLabel->setString(StringUtils::format("STATUS: LEVEL GENERATED, WAITING FOR START"));
//        m_eState = PreGameScene::State::READY_TO_START;
//    }
//    else if(m_eState == PreGameScene::State::READY_TO_START)
//    {
//        auto& socket = NetSystem::Instance().Socket();
//        GamePacket start_pack;
//        socket.receiveBytes(&start_pack, sizeof(start_pack));
//        m_pStatusLabel->setString(StringUtils::format("STATUS: STARTING GAME"));
//        m_eState = PreGameScene::State::ENTERING_GAME;
//    }
//    else if(m_eState == PreGameScene::State::ENTERING_GAME)
//    {
//        GameSceneSettings::Instance().SetGWPtr(m_pGameWorld);
//        auto game_scene = GameScene::createScene();
//        Director::getInstance()->replaceScene(game_scene);
//    }
}
