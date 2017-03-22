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
    
    m_pTakeItem = Sprite::create("res/hand.png");
    m_pTakeItem->setScale(0.4);
    m_pTakeItem->setPosition(size.width * 0.94,
                      size.height * 0.1);
    this->addChild(m_pTakeItem);
    
    m_pSpell1CD = Label::createWithTTF("Spell 1 CD:",
                                       "fonts/kenvector_future.ttf",
                                       20);
    m_pSpell1CD->setPosition(size.width * 0.20,
                             size.height * 0.4);
    this->addChild(m_pSpell1CD);
    
    m_pNetStatus = Label::createWithTTF("Ping:",
                                        "fonts/kenvector_future.ttf",
                                        20);
    m_pNetStatus->setPosition(size.width * 0.2,
                              size.height * 0.3);
    this->addChild(m_pNetStatus);
    
    m_pHP = Label::createWithTTF("HP:",
                                 "fonts/kenvector_future.ttf",
                                 20);
    m_pHP->setPosition(size.width * 0.20,
                       size.height * 0.97);
    this->addChild(m_pHP);
    
    m_pDMG = Label::createWithTTF("DMG:",
                                 "fonts/kenvector_future.ttf",
                                 20);
    m_pDMG->setPosition(size.width * 0.15,
                        size.height * 0.90);
    this->addChild(m_pDMG);
    
    m_pInventory = Label::createWithTTF("Inventory:",
                                        "fonts/kenvector_future.ttf",
                                        20);
    m_pInventory->setPosition(size.width * 0.85,
                              size.height * 0.90);
    this->addChild(m_pInventory);
}

GameHUD::~GameHUD()
{
    
}
