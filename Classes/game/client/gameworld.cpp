//
//  gameworld.cpp
//  labyrinth
//
//  Created by Aleksandr Borzikh on 15.04.17.
//
//

#include "gameworld.hpp"

#include "pregamescene.hpp"

#include "construction.hpp"
#include "item.hpp"
#include "units/units_inc.hpp"
#include "gameconfig.hpp"

#include <memory>
#include <string>


GameWorld::GameWorld(GameSessionDescriptor& descriptor)
: _mapConf(descriptor.MapConf),
  _view(cocos2d::Layer::create()),
  _objectsStorage(*this),
  _objectsLayer(cocos2d::Layer::create())
{
    _view->retain(); // release in destructor!
    _objectsLayer->retain();

    GameMap().GenerateMap(descriptor.MapConf, this);

    for(auto& player : descriptor.Players)
    {
        std::shared_ptr<Hero> hero;
        switch(player.Hero)
        {
        case Hero::Type::WARRIOR:
        {
            hero = _objectsStorage.CreateWithUID<Warrior>(player.Uid, "unit_warrior.png");
            break;
        }

        case Hero::Type::MAGE:
        {
            hero = _objectsStorage.CreateWithUID<Mage>(player.Uid, "unit_mage.png");
            break;
        }
        default:
        {
            assert(false);
            break;
        }
        }

        hero->SetName(player.Name);
        _objectsLayer->addChild(hero->GetSprite(), 10);

        if(player.Uid == descriptor.LocalPlayer.Uid)
        {
            _localPlayer = hero;
            hero->SetIsLocalPlayer(true);
        }
    }
}

void
GameWorld::ApplyInputMessages()
{
    while(!_inputMessages.empty())
    {
        auto packet = _inputMessages.front();
        _inputMessages.pop();

        auto message = GameMessage::GetMessage(packet.data());
        
        switch(message->payload_type())
        {
        case GameMessage::Messages_SVSpawnPlayer:
        {
            auto sv_spawn = static_cast<const GameMessage::SVSpawnPlayer*>(message->payload());

            auto player = _objectsStorage.FindObject<Unit>(sv_spawn->player_uid());
            player->Spawn(Point<>(sv_spawn->x(), sv_spawn->y()));
            
            break;
        }
            
        case GameMessage::Messages_SVSpawnMonster:
        {
            auto sv_spawn = static_cast<const GameMessage::SVSpawnMonster*>(message->payload());
            
            auto monster = _objectsStorage.CreateWithUID<Monster>(sv_spawn->monster_uid(), "unit_skeleton.png");
            monster->Spawn(Point<>(sv_spawn->x(), sv_spawn->y()));
            _objectsStorage.PushObject(monster);
            _objectsLayer->addChild(monster->GetSprite(), 10);
            _ui->m_pBattleLogs->AddLogMessage("Skeleton spawned");
            
            break;
        }
            
        case GameMessage::Messages_SVSpawnItem:
        {
            auto sv_spawn = static_cast<const GameMessage::SVSpawnItem*>(message->payload());

            switch((Item::Type)sv_spawn->item_type())
            {
            case Item::Type::KEY:
            {
                auto key = _objectsStorage.CreateWithUID<Key>(sv_spawn->item_uid(), "item_key.png");
                key->Spawn(Point<>(sv_spawn->x(), sv_spawn->y()));
                _objectsStorage.PushObject(key);
                _objectsLayer->addChild(key->GetSprite(), 0);
                break;
            }
                
            default:
                assert(false);
                break;
            }
            
            break;
        }
            
        case GameMessage::Messages_SVSpawnConstr:
        {
            auto sv_spawn = static_cast<const GameMessage::SVSpawnConstr*>(message->payload());
            
            switch((Construction::Type)sv_spawn->constr_type())
            {
            case Construction::Type::GRAVEYARD:
            {
                auto grave = _objectsStorage.CreateWithUID<Graveyard>(sv_spawn->constr_uid(), "construction_graveyard.png");
                grave->Spawn(Point<>(sv_spawn->x(), sv_spawn->y()));
                _objectsLayer->addChild(grave->GetSprite(), 0);
                break;
            }
                
            case Construction::Type::DOOR:
            {
                auto door = _objectsStorage.CreateWithUID<Door>(sv_spawn->constr_uid(), "construction_door.png");
                door->Spawn(Point<>(sv_spawn->x(), sv_spawn->y()));
                _objectsLayer->addChild(door->GetSprite(), 0);
                break;
            }

            case Construction::Type::FOUNTAIN:
            {
                auto fountain = _objectsStorage.CreateWithUID<Fountain>(sv_spawn->constr_uid(), "construction_fontaine_3.png");
                fountain->Spawn(Point<>(sv_spawn->x(), sv_spawn->y()));
                _objectsLayer->addChild(fountain->GetSprite(), 0);
                break;
            }
                
            default:
                assert(false);
                break;
            }
            
            break;
        }
            
        case GameMessage::Messages_SVActionMove:
        {
            auto sv_mov = static_cast<const GameMessage::SVActionMove*>(message->payload());

            auto unit = _objectsStorage.FindObject<Unit>(sv_mov->target_uid());
            unit->Move(sv_mov);

            break;
        }
            
        case GameMessage::Messages_SVActionItem:
        {
            auto sv_item = static_cast<const GameMessage::SVActionItem*>(message->payload());
            
            switch(sv_item->act_type())
            {
            case GameMessage::ActionItemType_TAKE:
            {
                auto item = _objectsStorage.FindObject<Item>(sv_item->item_uid());
                auto unit = _objectsStorage.FindObject<Unit>(sv_item->player_uid());

                item->Destroy();
                unit->TakeItem(item);
                
                if(item->GetType() == Item::Type::KEY)
                    _ui->m_pBattleLogs->AddLogMessage("Someone took the key");
                
                break;
            }
                
            default:
                assert(false);
                break;
            }
            
            break;
        }
            
        case GameMessage::Messages_SVActionDuel:
        {
            auto sv_duel = static_cast<const GameMessage::SVActionDuel*>(message->payload());
            
            switch(sv_duel->act_type())
            {
            case GameMessage::ActionDuelType_STARTED:
            {
                auto first = _objectsStorage.FindObject<Unit>(sv_duel->target1_uid());
                auto second = _objectsStorage.FindObject<Unit>(sv_duel->target2_uid());
                
                if(first->GetDuelTarget() != second &&
                   second->GetDuelTarget() != first)
                {
                    first->StartDuel(second);
                    second->StartDuel(first);
                }
                break;
            }
                
            default:
                assert(false);
                break;
            }
            
            break;
        }
            
        case GameMessage::Messages_SVActionDeath:
        {
            auto sv_death = static_cast<const GameMessage::SVActionDeath*>(message->payload());

            auto unit = _objectsStorage.FindObject<Unit>(sv_death->player_uid());
            unit->Die();

            _ui->m_pBattleLogs->AddLogMessage(cocos2d::StringUtils::format("%s died",
                                                                           unit->GetName().c_str()));
            break;
        }
            
        case GameMessage::Messages_SVRespawnPlayer:
        {
            auto sv_resp = static_cast<const GameMessage::SVRespawnPlayer*>(message->payload());

            auto player = _objectsStorage.FindObject<Unit>(sv_resp->player_uid());
            player->Respawn(Point<>(sv_resp->x(), sv_resp->y()));

            break;
        }
            
        case GameMessage::Messages_SVGameEnd:
        {
            auto sv_end = static_cast<const GameMessage::SVGameEnd*>(message->payload());

            auto winner = _objectsStorage.FindObject<Unit>(sv_end->player_uid());
            
                // game ends!
            auto winner_inf_pos = cocos2d::ui::RelativeLayoutParameter::create();
            winner_inf_pos->setAlign(cocos2d::ui::RelativeLayoutParameter::RelativeAlign::CENTER_IN_PARENT);
            
            auto winner_info = cocos2d::ui::Text::create(cocos2d::StringUtils::format("%s escaped... but others didn't",
                                                                                      winner->GetName().c_str()),
                                                         "fonts/alagard.ttf",
                                                         24);
            winner_info->setCameraMask((unsigned short)cocos2d::CameraFlag::USER1);
            winner_info->setLayoutParameter(winner_inf_pos);
            _ui->addChild(winner_info);
            
            _objectsLayer->setCascadeOpacityEnabled(true); // TODO: should be placed in init
            auto fade_out = cocos2d::FadeOut::create(5.0f);
            auto pop_scene = cocos2d::CallFunc::create([this]()
                                                       {
                                                           _view->release();
                                                           cocos2d::Director::getInstance()->popScene();
                                                       });
            auto seq = cocos2d::Sequence::create(fade_out,
                                                 cocos2d::DelayTime::create(2.0f),
                                                 pop_scene,
                                                 nullptr);
            _objectsLayer->runAction(seq);
            
            break;
        }
            
        case GameMessage::Messages_SVActionSpell:
        {
            auto sv_spell = static_cast<const GameMessage::SVActionSpell*>(message->payload());

            auto unit = _objectsStorage.FindObject<Unit>(sv_spell->player_uid());
            unit->SpellCast(sv_spell);

            break;
        }

        case GameMessage::Messages_SVPing:
        {
            flatbuffers::FlatBufferBuilder builder;
            auto uuid = builder.CreateString(GameConfiguration::Instance().GetUUID());
            auto ping = GameMessage::CreateCLPing(builder,
                                                  _localPlayer->GetUID());
            auto msg = GameMessage::CreateMessage(builder,
                                                  uuid,
                                                  GameMessage::Messages_CLPing,
                                                  ping.Union());
            builder.Finish(msg);

            _outEvents.emplace(builder.GetBufferPointer(),
                               builder.GetBufferPointer() + builder.GetSize());
            break;
        }
            
        default:
            assert(false);
            break;
        }
    }
}


void
GameWorld::update(float delta)
{
    ApplyInputMessages();
    for(auto iter = _objectsStorage.Begin(); iter != _objectsStorage.End(); ++iter)
        (*iter)->update(delta);

    // update camera pos
    auto cur_pos = _localPlayer->GetSprite()->getPosition();
    _camera->setPosition3D(cocos2d::Vec3(cur_pos.x, cur_pos.y, 800));
}

void
GameWorld::InitView()
{
    auto screenSize = cocos2d::Director::getInstance()->getWinSizeInPixels();
    auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();

    _objectsLayer->setAnchorPoint(cocos2d::Vec2::ZERO);

    _tempLayer = cocos2d::Layer::create();
    _tempLayer->addChild(_objectsLayer, 0);
    _view->addChild(_tempLayer);

    _tempLayer->setCameraMask((unsigned short)cocos2d::CameraFlag::USER2);

    auto frameBuffer = cocos2d::experimental::FrameBuffer::create(0, screenSize.width, screenSize.height);
    _renderTarget = cocos2d::experimental::RenderTarget::create(screenSize.width, screenSize.height);

    frameBuffer->attachRenderTarget(_renderTarget);
    frameBuffer->setClearColor(cocos2d::Color4F::BLACK);
    _renderTarget->getTexture()->setAliasTexParameters();

    auto depth = cocos2d::experimental::RenderTargetDepthStencil::create(screenSize.width, screenSize.height);
    frameBuffer->attachDepthStencilTarget(depth);

    _postProcess = cocos2d::Sprite::createWithTexture(_renderTarget->getTexture());
    cocos2d::GLProgramCache::getInstance()->addGLProgram(
                                                         cocos2d::GLProgram::createWithFilenames("res/graphics/shaders/Basic.vsh", "res/graphics/shaders/Light.fsh"),
                                                         "@shader"
                                                         );
    _postProcess->setGLProgram(cocos2d::GLProgramCache::getInstance()->getGLProgram("@shader"));
    _postProcess->getGLProgramState()->setUniformVec2("resolution", visibleSize);
    _postProcess->setFlippedY(true);
    _postProcess->setPosition(visibleSize.width / 2, visibleSize.height / 2);

    _view->addChild(_postProcess);

    _camera = cocos2d::Camera::create();
    _camera->setFrameBufferObject(frameBuffer);
    _camera->setCameraFlag(cocos2d::CameraFlag::USER2);
    _view->addChild(_camera);
}

void
GameWorld::SetHUD(UIGameScene * ui)
{
    _ui = ui;
    _localPlayer->SetHUD(ui);;
}
