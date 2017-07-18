//
//  settingsscene.cpp
//  labyrinth
//
//  Created by Aleksandr Borzikh on 11.04.17.
//
//

#include "settingsscene.hpp"

#include "gameconfig.hpp"

USING_NS_CC;

Scene *
SettingsScene::createScene()
{
    auto scene = Scene::create();
    auto layer = SettingsScene::create();
    scene->addChild(layer);
    
    return scene;
}

bool
SettingsScene::init()
{
    if(!Layer::init())
    {
        return false;
    }
    
    m_pUI = new UISettingsScene();
    this->addChild(m_pUI);
    
    auto back_button_callback = [this](Ref * pSender, ui::Button::TouchEventType type)
    {
        if(type == ui::Button::TouchEventType::ENDED)
        {
            GameConfiguration::Instance().SetPlayerName(m_pUI->m_pNicknameField->getString());
            
            GameConfiguration::Instance().Save();
            Director::getInstance()->popScene();
        }
    };
    m_pUI->m_pBackButton->addTouchEventListener(back_button_callback);
    
    auto& config = GameConfiguration::Instance();
    if(config.GetPlayerName() != "")
    {
        m_pUI->m_pNicknameField->setString(config.GetPlayerName());
    }
    
    return true;
}
