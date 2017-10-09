//
//  GameSearchScene.cpp
//  labyrinth
//
//  Created by Alexandr Borzykh on 13/09/2017.
//
//

#include "scenes/gamesearch/GameSearchScene.hpp"

#include "globals.h"
#include "gameconfig.hpp"
#include "game/client/gamemap.hpp"
#include "flat_generated/msnet_generated.h"
#include "flat_generated/gsnet_generated.h"

#include "toolkit/make_autorelease.hpp"

using cocos2d::Director;


namespace labyrinth
{

    bool GameSearchScene::init()
    {
        if (!Scene::init())
            return false;

        _ui = make_autorelease<ui::GameSearch>();
        this->addChild(_ui);

        _ui->GetSearchView()->SetStatus("Connecting to network");

        // Create socket, and send find game request
        _masterSocket = std::make_shared<UdpSocket>(Poco::Net::SocketAddress("localhost:1930"));
        _masterSocketConnection = _masterSocket->OnMessageConnector(std::bind(&GameSearchScene::MasterMessageHandler, this, std::placeholders::_1));

        flatbuffers::FlatBufferBuilder builder;
        auto uuid = builder.CreateString(GameConfiguration::Instance().GetUUID());
        auto req_lobby = MasterMessage::CreateCLFindGame(builder,
                                                         0/*GameConfiguration::Instance().GetUUID()*/,
                                                         GAMEVERSION_MAJOR,
                                                         GAMEVERSION_MINOR,
                                                         GAMEVERSION_BUILD);
        auto ms_event = MasterMessage::CreateMessage(builder,
                                                     uuid,
                                                     MasterMessage::Messages_CLFindGame,
                                                     req_lobby.Union());
        builder.Finish(ms_event);

        _masterSocket->SendMessage(CONSTRUCT_MESSAGEBUFFER(builder));

        this->scheduleUpdate();

        return true;
    }


    void GameSearchScene::update(float delta)
    {
        MessageStorage messages;
        {
            std::lock_guard<std::mutex> l(_mutex);
            messages.swap(_messages);
        }

        for (const auto& messageBuf : messages)
        {
            auto message = GameMessage::GetMessage(messageBuf->data());

            switch (message->payload_type())
            {
            case GameMessage::Messages_SVConnectionStatus:
            {
                const auto status = static_cast<const GameMessage::SVConnectionStatus*>(message->payload());

                if (status->status() == GameMessage::ConnectionStatus_ACCEPTED)
                {
                    _ui->GetSearchView()->SetStatus("Server accepted connection");
                    _ui->SwitchTo(ui::GameSearch::View::Lobby);

                    const PlayerConnection connection(GameConfiguration::Instance().GetPlayerName(), status->player_uid());
                    _playerConnections.push_back(connection);
                    _ui->GetLobbyView()->GetPlayersList()->InsertPlayer(connection.Name, connection.Uuid);

                    _selfConnection = connection;
                }
                else
                {
                    cocos2d::MessageBox("Error", "Lobby is full, try searching again");
                    cocos2d::Director::getInstance()->popScene();
                }
                
                break;
            }
            case GameMessage::Messages_SVPlayerConnected:
            {
                const auto player = static_cast<const GameMessage::SVPlayerConnected*>(message->payload());

                const PlayerConnection connection(player->nickname()->c_str(), player->player_uid());

                // check if player exists
                const auto iter = std::find_if(_playerConnections.cbegin(), _playerConnections.cend(),
                                               [&](const PlayerConnection& playerCon)
                                               {
                                                   if (playerCon.Uuid == connection.Uuid)
                                                       return true;
                                                   return false;
                                               });

                if (iter == _playerConnections.cend())
                {
                    _playerConnections.push_back(connection);
                    _ui->GetLobbyView()->GetPlayersList()->InsertPlayer(player->nickname()->c_str(), player->player_uid());
                }

                break;
            }
            case GameMessage::Messages_SVHeroPickStage:
            {
                const auto playerRow = _ui->GetLobbyView()->GetPlayersList()->GetPlayerRow(_selfConnection.Uuid);
                _readyConnection = playerRow->OnReadyClickedConnector(std::bind(&GameSearchScene::ReadyClickedHandler, this));

                break;
            }
            case GameMessage::Messages_SVGenerateMap:
            {
                const auto genInfo = static_cast<const GameMessage::SVGenerateMap*>(message->payload());

                _ui->SwitchTo(ui::GameSearch::View::Loading);
                _ui->GetLoadingView()->SetStatus("Generating world");

                GameMap::Configuration config;
                config.nMapSize = genInfo->map_w();
                config.nRoomSize = genInfo->room_w();
                config.nSeed = genInfo->seed();

                break;
            }
            default:
                CCLOG("Currently unsupported event, discarded");
                break;
            }
        }
    }


    void GameSearchScene::MasterMessageHandler(const MessageBufferPtr& messageBuffer)
    {
        auto message = MasterMessage::GetMessage(messageBuffer->data());

        if (message->payload_type() == MasterMessage::Messages_SVGameFound)
        {
            _ui->GetSearchView()->SetStatus("Server found!");

            _socket = std::make_shared<TcpSocket>(Poco::Net::SocketAddress("localhost", 1946));
            _socketConnection = _socket->OnMessageConnector(std::bind(&GameSearchScene::MessageHandler, this, std::placeholders::_1));

            auto uuid = GameConfiguration::Instance().GetUUID();
            auto handshake_msg = std::make_shared<MessageBuffer>();
            std::copy(uuid.begin(), uuid.end(), std::back_inserter(*handshake_msg));

            _socket->SendMessage(handshake_msg);

            {
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
                _socket->SendMessage(CONSTRUCT_MESSAGEBUFFER(builder));
            }
        }
    }


    void GameSearchScene::ReadyClickedHandler()
    {
        flatbuffers::FlatBufferBuilder builder;
        const auto uuid = builder.CreateString(GameConfiguration::Instance().GetUUID());
        const auto ready_msg = GameMessage::CreateCLReadyToStart(builder, _selfConnection.Uuid);
        const auto message = GameMessage::CreateMessage(builder, uuid, GameMessage::Messages_CLReadyToStart, ready_msg.Union());

        builder.Finish(message);

        _socket->SendMessage(CONSTRUCT_MESSAGEBUFFER(builder));
    }

}
