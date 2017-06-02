//
//  gamescene.cpp
//  labyrinth
//
//  Created by Aleksandr Borzikh on 12.01.17.
//
//

#include "gamescene.hpp"

#include "globals.h"
#include "gameconfig.hpp"

#include <memory>
#include <SimpleAudioEngine.h>
#include "mainmenuscene.hpp"
#include "netsystem.hpp"
#include "gsnet_generated.h"

#include "gamelogic/units/units_inc.hpp"

USING_NS_CC;

GameScene::GameScene()
{
    m_pGWorld = new GameWorld;
}

GameScene::~GameScene()
{
    
}

void
GameScene::onExit()
{
    Layer::onExit();
}

/*
 * BUILDER API
 */
GameWorld *
GameScene::GetGameWorld()
{
    return m_pGWorld;
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
    m_pGWorld->setPosition(Vec2::ZERO);
    this->addChild(m_pGWorld);
    
        // init UI (aka HUD)
    m_pUI = new UIGameScene();
    m_pUI->m_poBattleView->setActive(false);
    this->addChild(m_pUI);
    
    m_pUI->ConfigureForHero(m_pGWorld->GetLocalPlayer());
    m_pUI->setCameraMask((unsigned short)CameraFlag::USER1);
    
        // from now on, HUD view is controlled by gameworld,
        // but input is controlled by gamescene
    m_pGWorld->SetHUD(m_pUI);
    
    auto hud_camera = Camera::create();
    hud_camera->setCameraFlag(CameraFlag::USER1);
    this->addChild(hud_camera);
    
    auto eventListener = EventListenerKeyboard::create();
    eventListener->onKeyPressed = [this](EventKeyboard::KeyCode keyCode, Event * event)
    {
        if(keyCode == EventKeyboard::KeyCode::KEY_LEFT_ARROW)
        {
            m_pGWorld->GetLocalPlayer()->ApplyInputEvent(InputEvent::SWIPE_LEFT);
        }
        else if(keyCode == EventKeyboard::KeyCode::KEY_RIGHT_ARROW)
        {
            m_pGWorld->GetLocalPlayer()->ApplyInputEvent(InputEvent::SWIPE_RIGHT);
        }
        else if(keyCode == EventKeyboard::KeyCode::KEY_UP_ARROW)
        {
            m_pGWorld->GetLocalPlayer()->ApplyInputEvent(InputEvent::SWIPE_UP);
        }
        else if(keyCode == EventKeyboard::KeyCode::KEY_DOWN_ARROW)
        {
            m_pGWorld->GetLocalPlayer()->ApplyInputEvent(InputEvent::SWIPE_DOWN);
        }
        else if(keyCode == EventKeyboard::KeyCode::KEY_Q)
        {
            m_pGWorld->GetLocalPlayer()->ApplyInputEvent(InputEvent::SPELL_CAST_0_CLICK);
        }
    };
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(eventListener, this);
    
    auto take_button_callback = [this](Ref * pSender, ui::Widget::TouchEventType type)
    {
        if(type == ui::Widget::TouchEventType::ENDED)
        {
            m_pGWorld->GetLocalPlayer()->ApplyInputEvent(InputEvent::TAKE_ITEM_BUTTON_CLICK);
        }
    };
    m_pUI->m_pTakeItemButton->addTouchEventListener(take_button_callback);
    
        // FIXME: would be nicer in a loop
    auto skill_1_callback = [this](Ref * pSender, ui::Widget::TouchEventType type)
    {
        if(type == ui::Widget::TouchEventType::ENDED)
        {
            m_pGWorld->GetLocalPlayer()->ApplyInputEvent(InputEvent::SPELL_CAST_0_CLICK);
        }
    };
    m_pUI->m_poSkillsPanel->m_aSkillsButtons[0]->addTouchEventListener(skill_1_callback);
    
    this->scheduleUpdate();
    return true;
}

void
GameScene::update(float delta)
{
    m_pGWorld->update(delta);
    UpdateView(delta);
}

void
GameScene::UpdateView(float delta)
{
        // update camera pos
    Hero * player = m_pGWorld->GetLocalPlayer();
    auto pCam = Camera::getDefaultCamera();
    auto cur_pos = pCam->getPosition3D();
    cur_pos.x = (player->getPosition().x - cur_pos.x)*delta*3 + cur_pos.x;
    cur_pos.y = (player->getPosition().y- cur_pos.y)*delta*3 + cur_pos.y;
    pCam->setPosition3D(Vec3(
                             cur_pos.x,
                             cur_pos.y,
                             700));
}
