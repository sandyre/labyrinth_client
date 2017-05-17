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
    m_pUI->setCameraMask((unsigned short)CameraFlag::USER1);
    this->addChild(m_pUI);
    
    auto hud_camera = Camera::create();
    hud_camera->setCameraFlag(CameraFlag::USER1);
    this->addChild(hud_camera);
    
    auto eventListener = EventListenerKeyboard::create();
    eventListener->onKeyPressed = [this](EventKeyboard::KeyCode keyCode, Event * event)
    {
        if(keyCode == EventKeyboard::KeyCode::KEY_LEFT_ARROW)
        {
            m_pGWorld->GetLocalPlayer()->AddInputEvent(InputEvent::SWIPE_LEFT);
        }
        else if(keyCode == EventKeyboard::KeyCode::KEY_RIGHT_ARROW)
        {
            m_pGWorld->GetLocalPlayer()->AddInputEvent(InputEvent::SWIPE_RIGHT);
        }
        else if(keyCode == EventKeyboard::KeyCode::KEY_UP_ARROW)
        {
            m_pGWorld->GetLocalPlayer()->AddInputEvent(InputEvent::SWIPE_UP);
        }
        else if(keyCode == EventKeyboard::KeyCode::KEY_DOWN_ARROW)
        {
            m_pGWorld->GetLocalPlayer()->AddInputEvent(InputEvent::SWIPE_DOWN);
        }
        else if(keyCode == EventKeyboard::KeyCode::KEY_Q)
        {
            m_pGWorld->GetLocalPlayer()->AddInputEvent(InputEvent::SPELL_CAST_1_CLICK);
        }
    };
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(eventListener, this);
    
    auto take_button_callback = [this](Ref * pSender, ui::Widget::TouchEventType type)
    {
        if(type == ui::Widget::TouchEventType::ENDED)
        {
            m_pGWorld->GetLocalPlayer()->AddInputEvent(InputEvent::TAKE_ITEM_BUTTON_CLICK);
        }
    };
    m_pUI->m_pTakeItemButton->addTouchEventListener(take_button_callback);
    
        // FIXME: would be nicer in a loop
    auto skill_1_callback = [this](Ref * pSender, ui::Widget::TouchEventType type)
    {
        if(type == ui::Widget::TouchEventType::ENDED)
        {
            m_pGWorld->GetLocalPlayer()->AddInputEvent(InputEvent::SPELL_CAST_1_CLICK);
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
                             800));
    UpdateHUD(delta);
}

void
GameScene::UpdateHUD(float delta)
{
    Hero * player = m_pGWorld->GetLocalPlayer();
        // update hud
    m_pUI->m_pHPText->setString(StringUtils::format("%d",
                                                    player->GetHealth()));
    m_pUI->m_pHPBar->setPercent(((float)player->GetHealth() / player->GetMaxHealth()) * 100.0f);
    m_pUI->m_pArmor->setString(StringUtils::format("Armor: %d",
                                                   player->GetArmor()));

    auto& msgs = player->GetHUDMessages();
    while(!msgs.empty())
    {
        auto& msg = msgs.front();
        switch(msg.msg_type)
        {
            case HUDMessage::MessageType::DUEL_START:
            {
                m_pUI->m_poBattleView->setActive(true);
                break;
            }
            case HUDMessage::MessageType::DUEL_END:
            {
                m_pUI->m_poBattleView->setActive(false);
                break;
            }
            case HUDMessage::DUEL_SEQ_CORRECT_INPUT:
            {
                break;
            }
            case HUDMessage::DUEL_SEQ_COMPLETED:
            {
                break;
            }
            case HUDMessage::DUEL_SEQ_WRONG_INPUT:
            {
                break;
            }
        }
        msgs.pop();
    }
    
        // update battle logs
    while(!m_pGWorld->GetBattleLogs().empty())
    {
        m_pUI->m_pBattleLogs->AddLogMessage(m_pGWorld->GetBattleLogs().front());
        m_pGWorld->GetBattleLogs().pop();
    }
    
        // update CDS
    if(player->isSpellCast1Ready())
        m_pUI->m_poSkillsPanel->m_aSkillsButtons[0]->setEnabled(true);
    else
        m_pUI->m_poSkillsPanel->m_aSkillsButtons[0]->setEnabled(false);
}
