//
//  gamescene.cpp
//  labyrinth
//
//  Created by Aleksandr Borzikh on 12.01.17.
//
//

#include "gamescene.hpp"

#include "gameconfig.hpp"
#include "game/client/units/units_inc.hpp"
#include "globals.h"
#include "gsnet_generated.h"
#include "netsystem.hpp"
#include "toolkit/SafePacketGetter.hpp"

#include <memory>


USING_NS_CC;

void
GameScene::onExit()
{
    Layer::onExit();
}

/*
 * BUILDER API
 */

void
GameScene::InitWorld(GameSessionDescriptor& descr)
{
    _world = new GameWorld(descr);
    _channel = NetSystem::Instance().GetChannel("gameserver");
}

bool
GameScene::init()
{
    if( !Layer::init() )
    {
        return false;
    }
    
    auto visibleSize = Director::getInstance()->getVisibleSize();
    
        // add gameworld
    _world->InitView();

    _world->GetView()->setPosition(Vec2::ZERO);
    this->addChild(_world->GetView());
    
        // init UI (aka HUD)
    _ui = new UIGameScene();
    _ui->m_poBattleView->setActive(false);
    _ui->setGlobalZOrder(100);
    this->addChild(_ui);
    
    _ui->ConfigureForHero(_world->GetLocalPlayer().get());
    _ui->setCameraMask((unsigned short)CameraFlag::USER1);
    
        // from now on, HUD view is controlled by gameworld,
        // but input is controlled by gamescene
    _world->SetHUD(_ui);
    
    auto hud_camera = Camera::create();
    hud_camera->setCameraFlag(CameraFlag::USER1);
    this->addChild(hud_camera);
    
    auto eventListener = EventListenerKeyboard::create();
    eventListener->onKeyPressed = [this](EventKeyboard::KeyCode keyCode, Event * event)
    {
        if(keyCode == EventKeyboard::KeyCode::KEY_LEFT_ARROW)
        {
            _world->GetLocalPlayer()->EnqueueInputEvent(InputEvent::SWIPE_LEFT);
        }
        else if(keyCode == EventKeyboard::KeyCode::KEY_RIGHT_ARROW)
        {
            _world->GetLocalPlayer()->EnqueueInputEvent(InputEvent::SWIPE_RIGHT);
        }
        else if(keyCode == EventKeyboard::KeyCode::KEY_UP_ARROW)
        {
            _world->GetLocalPlayer()->EnqueueInputEvent(InputEvent::SWIPE_UP);
        }
        else if(keyCode == EventKeyboard::KeyCode::KEY_DOWN_ARROW)
        {
            _world->GetLocalPlayer()->EnqueueInputEvent(InputEvent::SWIPE_DOWN);
        }
        else if(keyCode == EventKeyboard::KeyCode::KEY_Q)
        {
            _world->GetLocalPlayer()->EnqueueInputEvent(InputEvent::SPELL_CAST_0_CLICK);
        }
    };
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(eventListener, this);
    
    auto take_button_callback = [this](Ref * pSender, ui::Widget::TouchEventType type)
    {
        if(type == ui::Widget::TouchEventType::ENDED)
        {
            _world->GetLocalPlayer()->EnqueueInputEvent(InputEvent::TAKE_ITEM_BUTTON_CLICK);
        }
    };
    _ui->m_pTakeItemButton->addTouchEventListener(take_button_callback);
    
        // FIXME: would be nicer in a loop
    auto skill_1_callback = [this](Ref * pSender, ui::Widget::TouchEventType type)
    {
        if(type == ui::Widget::TouchEventType::ENDED)
        {
            _world->GetLocalPlayer()->EnqueueInputEvent(InputEvent::SPELL_CAST_0_CLICK);
        }
    };
    _ui->m_poSkillsPanel->m_aSkillsButtons[0]->addTouchEventListener(skill_1_callback);
    
    this->scheduleUpdate();
    return true;
}

void
GameScene::update(float delta)
{
    auto& outMessages = _world->GetOutgoingMessages();
    while(!outMessages.empty())
    {
        _channel->PushPacket(outMessages.front());
        outMessages.pop();
    }
    
    while(_channel->Available())
    {
        SafePacketGetter safeGetter(_channel->native_handler());
        auto packet = safeGetter.Get<GameMessage::Message>();

        if(!packet)
            continue;

        auto message = GameMessage::GetMessage(packet->Data.data());

        if(message->payload_type() == GameMessage::Messages_SVPing) // TODO: send ping back
            continue;

        _world->PushMessage({packet->Data.begin(), packet->Data.end()});
    }
    
    _world->update(delta);
    UpdateView(delta);
}

void
GameScene::UpdateView(float delta)
{
        
}
