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
#include "flat_generated/msnet_generated.h"
#include "flat_generated/gsnet_generated.h"

#include "toolkit/make_autorelease.hpp"

using cocos2d::Director;


namespace labyrinth
{

    GameSearchScene::GameSearchScene()
    { }


    bool GameSearchScene::init()
    {
        if (!Scene::init())
            return false;

        auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();

        _ui = make_autorelease<ui::GameSearch>();
        _ui->setPosition(visibleSize / 2);
        this->addChild(_ui);

        _ui->GetSearchView()->SetStatus("Finding available lobby");

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

        _masterSocket->SendMessage(std::make_shared<MessageBuffer>(builder.GetBufferPointer(),
                                                                   builder.GetBufferPointer() + builder.GetSize()));


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

                _playerConnections.emplace_back(player->nickname()->c_str(), player->player_uid());

                _ui->GetLobbyView()->GetPlayersList()->InsertPlayer(player->nickname()->c_str(), player->player_uid());

                break;
            }
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
                _socket->SendMessage(std::make_shared<MessageBuffer>(builder.GetBufferPointer(),
                                                                     builder.GetBufferPointer() + builder.GetSize()));
            }
        }
    }

}
