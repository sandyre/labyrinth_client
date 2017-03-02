//
//  ingamehud.cpp
//  labyrinth
//
//  Created by Aleksandr Borzikh on 08.02.17.
//
//

#include "gamehud.hpp"

USING_NS_CC;

GameHUD::GameHUD()
{
    auto size = Director::getInstance()->getVisibleSize();
    
    auto heart = Sprite::create("res/heart.png");
    heart->setScale(0.4);
    heart->setPosition(size.width * 0.056,
                       size.height * 0.97);
    this->addChild(heart);
    
    auto fist = Sprite::create("res/fist.png");
    fist->setScale(0.4);
    fist->setPosition(size.width * 0.056,
                       size.height * 0.90);
    this->addChild(fist);
    
    m_pTakeItem = Sprite::create("res/hand.png");
    m_pTakeItem->setScale(0.4);
    m_pTakeItem->setPosition(size.width * 0.94,
                      size.height * 0.1);
    this->addChild(m_pTakeItem);
    
    m_pState = Label::createWithTTF("",
                                    "fonts/Marker Felt.ttf",
                                    35);
    m_pState->setPosition(size.width * 0.5,
                          size.height * 0.9);
    this->addChild(m_pState);
    
    m_pHP = Label::createWithTTF("HP:",
                                 "fonts/Marker Felt.ttf",
                                 30);
    m_pHP->setPosition(size.width * 0.20,
                       size.height * 0.97);
    this->addChild(m_pHP);
    
    m_pDMG = Label::createWithTTF("DMG:",
                                 "fonts/Marker Felt.ttf",
                                 30);
    m_pDMG->setPosition(size.width * 0.15,
                        size.height * 0.90);
    this->addChild(m_pDMG);
    
    m_pInventory = Label::createWithTTF("Inventory:",
                                        "fonts/Marker Felt.ttf",
                                        30);
    m_pInventory->setPosition(size.width * 0.85,
                              size.height * 0.90);
    this->addChild(m_pInventory);
}

GameHUD::~GameHUD()
{
    
}
