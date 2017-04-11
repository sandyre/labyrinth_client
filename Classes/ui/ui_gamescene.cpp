//
//  ui_gamescene.cpp
//  labyrinth
//
//  Created by Aleksandr Borzikh on 09.04.17.
//
//

#include "ui_gamescene.hpp"

#include "../resources.hpp"
USING_NS_CC;

UIGameScene::UIGameScene()
{
    auto visible_size = Director::getInstance()->getVisibleSize();
    
        // layout init
    this->setLayoutType(ui::Layout::Type::RELATIVE);
    this->setContentSize(visible_size);
    
        // stats layout init
    auto stats_size = visible_size;
    stats_size.height *= 0.25;
    stats_size.width *= 0.25;
    
    auto stats_pos = ui::RelativeLayoutParameter::create();
    stats_pos->setAlign(ui::RelativeLayoutParameter::RelativeAlign::PARENT_TOP_LEFT);
    
    m_pStatsLayout = ui::Layout::create();
    m_pStatsLayout->setLayoutType(ui::Layout::Type::VERTICAL);
    m_pStatsLayout->setContentSize(stats_size);
    this->addChild(m_pStatsLayout);
    
        // init hp
    auto hp_pos = ui::LinearLayoutParameter::create();
    hp_pos->setGravity(ui::LinearLayoutParameter::LinearGravity::LEFT);
    
    m_pHP = ui::Text::create(" ",
                             TitleFont,
                             35);
    m_pHP->setAnchorPoint(Vec2::ZERO);
    m_pHP->setLayoutParameter(hp_pos);
    
    m_pStatsLayout->addChild(m_pHP);
    
        // init dmg
    auto dmg_pos = ui::LinearLayoutParameter::create();
    hp_pos->setGravity(ui::LinearLayoutParameter::LinearGravity::LEFT);
    
    m_pDamage = ui::Text::create(" ",
                             TitleFont,
                             35);
    m_pDamage->setAnchorPoint(Vec2::ZERO);
    m_pDamage->setLayoutParameter(dmg_pos);
    
    m_pStatsLayout->addChild(m_pDamage);
    
        // init selected item
    auto frame_pos = ui::RelativeLayoutParameter::create();
    frame_pos->setAlign(ui::RelativeLayoutParameter::RelativeAlign::PARENT_LEFT_BOTTOM);
    
    m_pItemFrame = ui::ImageView::create("res/frame.png");
    m_pItemFrame->setLayoutParameter(frame_pos);
    this->addChild(m_pItemFrame);
    
        // init take item button
    auto take_item_pos = ui::RelativeLayoutParameter::create();
    take_item_pos->setAlign(ui::RelativeLayoutParameter::RelativeAlign::PARENT_RIGHT_BOTTOM);
    
    m_pTakeItemButton = ui::Button::create("res/take_item.png");
    m_pTakeItemButton->setLayoutParameter(take_item_pos);
    this->addChild(m_pTakeItemButton);
}
