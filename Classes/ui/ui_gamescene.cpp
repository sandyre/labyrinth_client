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
                             30);
    m_pHP->setAnchorPoint(Vec2::ZERO);
    m_pHP->setLayoutParameter(hp_pos);
    
    m_pStatsLayout->addChild(m_pHP);
    
        // init dmg
    auto dmg_pos = ui::LinearLayoutParameter::create();
    hp_pos->setGravity(ui::LinearLayoutParameter::LinearGravity::LEFT);
    
    m_pDamage = ui::Text::create(" ",
                             TitleFont,
                             30);
    m_pDamage->setAnchorPoint(Vec2::ZERO);
    m_pDamage->setLayoutParameter(dmg_pos);
    
    m_pStatsLayout->addChild(m_pDamage);
    
        // init selected item
    auto frame_pos = ui::RelativeLayoutParameter::create();
    frame_pos->setAlign(ui::RelativeLayoutParameter::RelativeAlign::PARENT_LEFT_BOTTOM);
    
    m_pItemFrame = ui::ImageView::create("res/frame.png");
    m_pItemFrame->setLayoutParameter(frame_pos);
    this->addChild(m_pItemFrame);
    
    m_pSelectedItem = ui::ImageView::create();
    m_pSelectedItem->setLayoutParameter(frame_pos);
    this->addChild(m_pSelectedItem);
    
        // init take item button
    auto take_item_pos = ui::RelativeLayoutParameter::create();
    take_item_pos->setAlign(ui::RelativeLayoutParameter::RelativeAlign::PARENT_RIGHT_BOTTOM);
    
    m_pTakeItemButton = ui::Button::create("res/take_item.png");
    m_pTakeItemButton->setLayoutParameter(take_item_pos);
    this->addChild(m_pTakeItemButton);
    
        // init battle logs
    auto battle_logs_pos = ui::RelativeLayoutParameter::create();
    battle_logs_pos->setAlign(ui::RelativeLayoutParameter::RelativeAlign::PARENT_TOP_RIGHT);
    
    m_pBattleLogs = ui::Text::create("battle logs will be here",
                                     TitleFont,
                                     20);
    m_pBattleLogs->setLayoutParameter(battle_logs_pos);
    this->addChild(m_pBattleLogs);
    
        // init 'skillspanel'
    auto skills_panel_pos = ui::RelativeLayoutParameter::create();
    skills_panel_pos->setAlign(ui::RelativeLayoutParameter::RelativeAlign::PARENT_RIGHT_CENTER_VERTICAL);
    
    m_poSkillsPanel = new UISkillsPanel();
    m_poSkillsPanel->setLayoutParameter(skills_panel_pos);
    this->addChild(m_poSkillsPanel);
    
        // init 'battleview'
    auto battle_view_pos = ui::RelativeLayoutParameter::create();
    battle_view_pos->setAlign(ui::RelativeLayoutParameter::RelativeAlign::PARENT_LEFT_BOTTOM);
    
    m_poBattleView = new UIBattleView();
    m_poBattleView->setLayoutParameter(battle_view_pos);
    this->addChild(m_poBattleView);
}

UISkillsPanel::UISkillsPanel()
{
    auto visible_size = Director::getInstance()->getVisibleSize();
    
        // layout init
    auto layout_size = visible_size;
    layout_size.width *= 0.2;
    layout_size.height *= 0.3;
    
    this->setLayoutType(ui::Layout::Type::VERTICAL);
    this->setContentSize(layout_size);
    
        // init 'skills'
    for(int i = 0; i < 1; ++i)
    {
        auto skill_pos = ui::LinearLayoutParameter::create();
        skill_pos->setGravity(ui::LinearLayoutParameter::LinearGravity::TOP);
        
        auto skill_button = ui::Button::create("res/earth_elem.png");
        skill_button->setLayoutParameter(skill_pos);
        this->addChild(skill_button);
        m_aSkillsButtons.push_back(skill_button);
    }
}

UIBattleView::UIBattleView()
{
    auto visible_size = Director::getInstance()->getVisibleSize();
    
        // layout init
    this->setLayoutType(ui::Layout::Type::RELATIVE);
    this->setContentSize(visible_size);
    
        // init 'actionsview'
    auto act_view_pos = ui::RelativeLayoutParameter::create();
    act_view_pos->setAlign(ui::RelativeLayoutParameter::RelativeAlign::PARENT_LEFT_CENTER_VERTICAL);
    
    m_poActionsView = new UIActionsView();
    m_poActionsView->setLayoutParameter(act_view_pos);
    this->addChild(m_poActionsView);
}

void
UIBattleView::setActive(bool val)
{
    this->setEnabled(val);
    this->setVisible(val);
}

UIActionsView::UIActionsView()
{
    auto visible_size = Director::getInstance()->getVisibleSize();
    
        // layout init
    auto layout_size = visible_size;
    layout_size.width *= 0.5;
    layout_size.height *= 0.4;
    
    this->setLayoutType(ui::Layout::Type::VERTICAL);
    this->setContentSize(layout_size);
//    this->setBackGroundColorType(ui::Layout::BackGroundColorType::SOLID);
//    this->setBackGroundColor(Color3B::GRAY);
    
        // init actions
    for(int i = 0; i < 1; ++i)
    {
        auto action_pos = ui::LinearLayoutParameter::create();
        action_pos->setGravity(ui::LinearLayoutParameter::LinearGravity::RIGHT);
        
        auto action = new UIAction();
        action->setLayoutParameter(action_pos);
        this->addChild(action);
        m_apActions.push_back(action);
    }
}

UIAction::UIAction()
{
    auto visible_size = Director::getInstance()->getVisibleSize();
    
        // layout init
    auto layout_size = visible_size;
    layout_size.width *= 0.5;
    layout_size.height *= 0.1;
    
    this->setLayoutType(ui::Layout::Type::HORIZONTAL);
    this->setContentSize(layout_size);
    this->setBackGroundColorType(ui::Layout::BackGroundColorType::SOLID);
    this->setBackGroundColor(Color3B::GRAY);
    
        // init icon
    auto icon_pos = ui::LinearLayoutParameter::create();
    icon_pos->setGravity(ui::LinearLayoutParameter::LinearGravity::LEFT);
    
    m_pIcon = ui::ImageView::create("res/not_ready.png");
    m_pIcon->setLayoutParameter(icon_pos);
    this->addChild(m_pIcon);
}
