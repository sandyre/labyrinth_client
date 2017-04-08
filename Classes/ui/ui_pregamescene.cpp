//
//  ui_pregamescene.cpp
//  labyrinth
//
//  Created by Aleksandr Borzikh on 26.03.17.
//
//

#include "ui_pregamescene.hpp"

#include "../globals.h"
#include "../resources.hpp"

USING_NS_CC;

UIPregameScene::UIPregameScene()
{
        // layout init
    this->setLayoutType(ui::Layout::Type::RELATIVE);
    this->setContentSize(Director::getInstance()->getVisibleSize());
    this->setPosition(Vec2::ZERO);
    
        // status bar init
    auto status_position = ui::RelativeLayoutParameter::create();
    status_position->setAlign(ui::RelativeLayoutParameter::RelativeAlign::PARENT_TOP_CENTER_HORIZONTAL);
    
    m_pStatusText = ui::Text::create("Default status",
                                     "fonts/kenvector_future.ttf",
                                     24);
    m_pStatusText->setLayoutParameter(status_position);
    this->addChild(m_pStatusText);
    
        // hero pick init
    auto hero_pick_pos = ui::RelativeLayoutParameter::create();
    hero_pick_pos->setAlign(ui::RelativeLayoutParameter::RelativeAlign::PARENT_TOP_CENTER_HORIZONTAL);
    hero_pick_pos->setMargin(ui::Margin(0, m_pStatusText->getContentSize().height, 0, 0));
    
    m_pHeroPick = new UIHeroPick();
    m_pHeroPick->setLayoutParameter(hero_pick_pos);
    this->addChild(m_pHeroPick);
    
        // players list init
    auto players_list_pos = ui::RelativeLayoutParameter::create();
    players_list_pos->setAlign(ui::RelativeLayoutParameter::RelativeAlign::PARENT_BOTTOM_CENTER_HORIZONTAL);
    
    m_pPlayersList = new UIPlayersList();
    m_pPlayersList->setLayoutParameter(players_list_pos);
    this->addChild(m_pPlayersList);
}

UIHeroPick::UIHeroPick()
{
    this->setLayoutType(ui::Layout::Type::RELATIVE);
    this->setPosition(Vec2::ZERO);
    this->setBackGroundColorType(ui::Layout::BackGroundColorType::SOLID);
    this->setBackGroundColor(Color3B::GRAY);
    
    auto vis_size = Director::getInstance()->getVisibleSize();
    vis_size.height *= 0.45;
    this->setContentSize(vis_size);
    
        // selected hero text
    auto sel_hero_text = ui::RelativeLayoutParameter::create();
    sel_hero_text->setAlign(ui::RelativeLayoutParameter::RelativeAlign::PARENT_BOTTOM_CENTER_HORIZONTAL);
    
    m_pSelectedHeroText = ui::Text::create("No hero selected",
                                           "fonts/kenvector_future.ttf",
                                           16);
    m_pSelectedHeroText->setLayoutParameter(sel_hero_text);
    this->addChild(m_pSelectedHeroText);
    
        // selected hero image
    auto sel_hero_image = ui::RelativeLayoutParameter::create();
    sel_hero_image->setAlign(ui::RelativeLayoutParameter::RelativeAlign::CENTER_IN_PARENT);
    
    m_pSelectedHeroImage = ui::ImageView::create("res/no_hero.png");
    m_pSelectedHeroImage->setLayoutParameter(sel_hero_image);
    this->addChild(m_pSelectedHeroImage);
    
        // right button herochange init
    auto right_button_pos = ui::RelativeLayoutParameter::create();
    right_button_pos->setAlign(ui::RelativeLayoutParameter::RelativeAlign::PARENT_RIGHT_CENTER_VERTICAL);
    
    m_pRightChange = ui::Button::create("res/arrow_right.png");
    m_pRightChange->setLayoutParameter(right_button_pos);
    this->addChild(m_pRightChange);
    
        // left button herochange init
    auto left_button_pos = ui::RelativeLayoutParameter::create();
    left_button_pos->setAlign(ui::RelativeLayoutParameter::RelativeAlign::PARENT_LEFT_CENTER_VERTICAL);
    
    m_pLeftChange = ui::Button::create("res/arrow_left.png");
    m_pLeftChange->setLayoutParameter(left_button_pos);
    this->addChild(m_pLeftChange);
}

UIPlayerInfo::UIPlayerInfo()
{
    this->setLayoutType(ui::Layout::Type::HORIZONTAL);
    this->setPosition(Vec2::ZERO);
    
    auto vis_size = Director::getInstance()->getVisibleSize();
    vis_size.height *= 0.125;
    this->setContentSize(vis_size);
    
        // init hero icon
    auto image_param = ui::LinearLayoutParameter::create();
    image_param->setGravity(ui::LinearLayoutParameter::LinearGravity::CENTER_VERTICAL);
    
    m_pHeroImage = ui::ImageView::create("res/no_hero.png");
    m_pHeroImage->setLayoutParameter(image_param);
    this->addChild(m_pHeroImage);
    
        // init players name
    auto text_param = ui::LinearLayoutParameter::create();
    text_param->setGravity(ui::LinearLayoutParameter::LinearGravity::CENTER_VERTICAL);
    
    m_pPlayerName = ui::Text::create("unknown",
                                     "fonts/kenvector_future.ttf",
                                     16);
    m_pPlayerName->setLayoutParameter(text_param);
    this->addChild(m_pPlayerName);
    
        // init 'ready' checkbox
    auto ready_param = ui::LinearLayoutParameter::create();
    ready_param->setGravity(ui::LinearLayoutParameter::LinearGravity::CENTER_VERTICAL);
    
    m_pReadyStatus = ui::CheckBox::create("res/frame.png",
                                          "res/ready.png");
    m_pReadyStatus->setLayoutParameter(ready_param);
    m_pReadyStatus->setEnabled(false);
    this->addChild(m_pReadyStatus);
}

UIPlayersList::UIPlayersList()
{
    this->setLayoutType(ui::Layout::Type::RELATIVE);
    this->setBackGroundColorType(ui::Layout::BackGroundColorType::SOLID);
    this->setBackGroundColor(Color3B::GREEN);
    
    auto vis_size = Director::getInstance()->getVisibleSize();
    vis_size.height *= 0.45;
    
    this->setContentSize(vis_size);
    
        // init 'players' label
    auto players_label_pos = ui::RelativeLayoutParameter::create();
    players_label_pos->setAlign(ui::RelativeLayoutParameter::RelativeAlign::PARENT_TOP_CENTER_HORIZONTAL);
    
    m_pPlayersLabel = ui::Text::create("Players list",
                                       "fonts/kenvector_future.ttf",
                                       26);
    m_pPlayersLabel->setLayoutParameter(players_label_pos);
    this->addChild(m_pPlayersLabel);
    
        // init players list view
    auto list_size = vis_size;
    list_size.height -= m_pPlayersLabel->getContentSize().height;
    
    auto players_list_pos = ui::RelativeLayoutParameter::create();
    players_list_pos->setAlign(ui::RelativeLayoutParameter::RelativeAlign::CENTER_IN_PARENT);
    
    m_pListView = ui::ListView::create();
    m_pListView->setLayoutParameter(players_list_pos);
    m_pListView->setDirection(ui::ScrollView::Direction::VERTICAL);
    m_pListView->setContentSize(list_size);
    this->addChild(m_pListView);
}

void
UIPlayersList::AddPlayer(const PlayerInfo& player)
{
    UIPlayerInfo * pPlayer = new UIPlayerInfo();
    pPlayer->m_stPlayerInfo = player;
    pPlayer->m_pPlayerName->setString(StringUtils::format("%s#%d",
                                                          player.sNickname.c_str(),
                                                          player.nUID));
    m_pListView->pushBackCustomItem(pPlayer);
    m_aPlayers.push_back(pPlayer);
}

void
UIPlayersList::RemovePlayer(uint32_t uid)
{
        // TODO: remove required UIPlayerInfo
}
