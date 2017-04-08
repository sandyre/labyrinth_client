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
    
    m_pUI->m_pPlayButton->addTouchEventListener([this](Ref * pSender, ui::Widget::TouchEventType type)
                                                {
                                                    if(type == ui::Widget::TouchEventType::ENDED)
                                                    {
                                                        Director::getInstance()->pushScene(PreGameScene::createScene());
                                                    }
                                                });
    
    return true;
}
