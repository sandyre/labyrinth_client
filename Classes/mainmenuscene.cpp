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
    
    auto visibleSize = Director::getInstance()->getVisibleSize();
    
    m_pLayout = ui::Layout::create();
    m_pLayout->setLayoutType(ui::Layout::Type::RELATIVE);
    m_pLayout->setPosition(Vec2::ZERO);
    m_pLayout->setContentSize(visibleSize);
    m_pLayout->setBackGroundColorType(ui::Layout::BackGroundColorType::SOLID);
    m_pLayout->setBackGroundColor(Color3B(37, 68, 53));
//    m_pLayout->setBackGroundImage("res/mainmenu_back.png");
    this->addChild(m_pLayout);
    
    auto layout_par1 = ui::RelativeLayoutParameter::create();
    layout_par1->setAlign(ui::RelativeLayoutParameter::RelativeAlign::PARENT_TOP_LEFT);
    layout_par1->setMargin(ui::Margin(10.0, 2.0, 0.0, 0.0));
    
    auto playerInfo = ui::Text::create(StringUtils::format(
                                                           "logged as %s\nUID:%d",
                                                           AccountInfo::Instance().GetNickname().c_str(),
                                                           AccountInfo::Instance().GetUID()),
                                       "fonts/kenvector_future.ttf", 20);
    playerInfo->setLayoutParameter(layout_par1);
    m_pLayout->addChild(playerInfo);
    
    auto layout_par2 = ui::RelativeLayoutParameter::create();
    layout_par2->setAlign(ui::RelativeLayoutParameter::RelativeAlign::PARENT_TOP_CENTER_HORIZONTAL);
    layout_par2->setMargin(ui::Margin(0, 100, 0, 0));
    
    auto titleLabel = ui::Text::create("labyrinth reborn",
                                       "fonts/jigsaw trouserdrop.ttf",
                                       40);
    titleLabel->setLayoutParameter(layout_par2);
    titleLabel->enableShadow();
    titleLabel->enableOutline(Color4B::RED);
    titleLabel->enableGlow(Color4B::RED);
    m_pLayout->addChild(titleLabel);
    
    auto layout_par3 = ui::RelativeLayoutParameter::create();
    layout_par3->setAlign(ui::RelativeLayoutParameter::RelativeAlign::PARENT_TOP_RIGHT);
    
    auto gameVersion = ui::Text::create(StringUtils::format("v%d.%d.%d",
                                                            GAMEVERSION_MAJOR,
                                                            GAMEVERSION_MINOR,
                                                            GAMEVERSION_BUILD),
                                        "fonts/kenvector_future.ttf", 16);
    gameVersion->setLayoutParameter(layout_par3);
    m_pLayout->addChild(gameVersion);
    
    auto layout_par4 = ui::RelativeLayoutParameter::create();
    layout_par4->setAlign(ui::RelativeLayoutParameter::RelativeAlign::CENTER_IN_PARENT);
    
    auto menuLayout = ui::Layout::create();
    menuLayout->setLayoutType(ui::Layout::Type::VERTICAL);
    menuLayout->setContentSize(Size(400, 400)); // FIXME: hardcoded
    menuLayout->setLayoutParameter(layout_par4);
    m_pLayout->addChild(menuLayout);
    
    auto button_par1 = ui::LinearLayoutParameter::create();
    button_par1->setGravity(ui::LinearLayoutParameter::LinearGravity::CENTER_HORIZONTAL);
    
    auto playButton = ui::Button::create("res/button_normal.png",
                                         "res/button_pressed.png");
    playButton->setTitleText("Play");
    playButton->setTitleFontName("fonts/kenvector_future.ttf");
    playButton->setTitleFontSize(16);
    playButton->setLayoutParameter(button_par1);
    playButton->addTouchEventListener([](Ref * pSender, ui::Widget::TouchEventType type)
                                      {
                                          if(type == ui::Widget::TouchEventType::ENDED)
                                          {
                                              Director::getInstance()->pushScene(PreGameScene::createScene());
                                          }
                                      });
    menuLayout->addChild(playButton);
    
    auto button_par2 = ui::RelativeLayoutParameter::create();
    button_par2->setAlign(ui::RelativeLayoutParameter::RelativeAlign::PARENT_RIGHT_BOTTOM);
    
    auto settButton = ui::Button::create("res/settings.png");
    settButton->setTitleText("Settings");
    settButton->setTitleFontName("fonts/kenvector_future.ttf");
    settButton->setTitleFontSize(16);
    settButton->setLayoutParameter(button_par2);
    m_pLayout->addChild(settButton);
    
    auto button_par3 = ui::RelativeLayoutParameter::create();
    button_par3->setAlign(ui::RelativeLayoutParameter::RelativeAlign::PARENT_LEFT_BOTTOM);
    
    auto donateButton = ui::Button::create("res/donate.png");
    donateButton->setTitleText("Donate");
    donateButton->setTitleFontName("fonts/kenvector_future.ttf");
    donateButton->setTitleFontSize(16);
    donateButton->setLayoutParameter(button_par3);
    m_pLayout->addChild(donateButton);
    
    auto logo_par1 = ui::RelativeLayoutParameter::create();
    logo_par1->setAlign(ui::RelativeLayoutParameter::RelativeAlign::PARENT_BOTTOM_CENTER_HORIZONTAL);
    
    auto logo = ui::ImageView::create("res/hatered.png");
    logo->setLayoutParameter(logo_par1);
    m_pLayout->addChild(logo);

    return true;
}
