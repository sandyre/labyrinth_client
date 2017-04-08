    //
    //  mainmenuscene.cpp
    //  labyrinth
    //
    //  Created by Aleksandr Borzikh on 20.01.17.
    //
    //

#include "mainmenuscene.hpp"

#include "pregamescene.hpp"

#include "accountinfo.hpp"
#include "netsystem.hpp"
#include "msnet_generated.h"
#include <fstream>
#include <string>
#include <Poco/Net/DatagramSocket.h>

USING_NS_CC;

Scene *
MainMenuScene::createScene()
{
    auto scene = Scene::create();
    auto layer = MainMenuScene::create();
    scene->addChild(layer);
    
    return scene;
}

bool
MainMenuScene::init()
{
    if( !Layer::init() )
    {
        return false;
    }
    
    m_pUI = new UIMainMenuScene();
    this->addChild(m_pUI);
    
        // listener to get into login page
    m_pUI->m_pStartPage->m_pStartButton->addTouchEventListener([this](Ref * pSender, ui::Widget::TouchEventType type)
                                                               {
                                                                   if(type == ui::Widget::TouchEventType::ENDED)
                                                                   {
                                                                       m_pUI->m_pPageView->scrollToPage(1);
                                                                   }
                                                               });
    
        // listener to get into main page
    m_pUI->m_pLoginPage->m_pLogInButton->addTouchEventListener([this](Ref * pSender, ui::Widget::TouchEventType type)
                                                               {
                                                                   if(type == ui::Widget::TouchEventType::ENDED)
                                                                   {
                                                                       m_pUI->m_pPageView->scrollToPage(2);
                                                                   }
                                                               });
    
        // listener to get into play pregamescene
    m_pUI->m_pMainPage->m_pPlayButton->addTouchEventListener([this](Ref * pSender, ui::Widget::TouchEventType type)
                                                             {
                                                                 if(type == ui::Widget::TouchEventType::ENDED)
                                                                 {
                                                                     Director::getInstance()->pushScene(PreGameScene::createScene());
                                                                 }
                                                             });
    
    return true;
}
