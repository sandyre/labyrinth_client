//
//  mainmenuscene.cpp
//  labyrinth
//
//  Created by Aleksandr Borzikh on 20.01.17.
//
//

#include "mainmenuscene.hpp"

#include "pregamescene.hpp"

#include "playerinfo.hpp"
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
    
        // read MS addr
    std::string ms_addr_t;
    std::ifstream net_file("res/net_info.txt");
    std::getline(net_file,ms_addr_t);
    net_file.close();
    
    Poco::Net::SocketAddress ms_addr(ms_addr_t);
    NetSystem::Instance().Socket().connect(ms_addr);
    
    auto visibleSize = Director::getInstance()->getVisibleSize();
    
    auto playerInfo = Label::createWithTTF(StringUtils::format(
                                                               "logged as %s\nUID:%d",
                                                               PlayerInfo::Instance().GetNickname().c_str(),
                                                               PlayerInfo::Instance().GetUID()),
                                             "fonts/ethnocentric rg.ttf", 16);
    playerInfo->setPosition(visibleSize.width * 0.8, visibleSize.height * 0.95);
    this->addChild(playerInfo);
    
    auto titleLabel = Label::createWithTTF(
                                           "labyrinth reborn",
                                           "fonts/jigsaw trouserdrop.ttf",
                                           40);
    titleLabel->setPosition(visibleSize.width/2, visibleSize.height * 0.8);
    titleLabel->enableShadow();
    titleLabel->enableOutline(Color4B::RED);
    titleLabel->enableGlow(Color4B::RED);
    this->addChild(titleLabel);
    
    auto gameLabel = Label::createWithTTF(
                                          "find game",
                                          "fonts/ethnocentric rg.ttf",
                                          32);
    auto gameMItem = MenuItemLabel::create(gameLabel,
                                           CC_CALLBACK_1(
                                                         MainMenuScene::enterGameCallback,
                                                         this));
    gameMItem->setPosition(Vec2::ZERO);
    
    auto menu = Menu::create(gameMItem, nullptr);
    menu->setPosition(visibleSize.width/2, visibleSize.height/2);
    
    this->addChild(menu, 1);
    
    auto logo = Sprite::create("res/hatered.png");
    logo->setPosition(visibleSize.width * 0.5,
                      visibleSize.height * 0.1);
    logo->setScale(2.0);
    this->addChild(logo);
    
    return true;
}

void
MainMenuScene::enterGameCallback(Ref * pSender)
{
    Director::getInstance()->pushScene(PreGameScene::createScene());
}
