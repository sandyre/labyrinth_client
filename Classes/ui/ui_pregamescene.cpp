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

UIPregameScene::UIPregameScene() :
m_pStatusText(ui::Text::create()),
m_pPlayersList(new UIPlayersList()),
m_pHeroPick(new UIHeroPick())
{
        // layout init
    this->setLayoutType(ui::Layout::Type::RELATIVE);
    this->setContentSize(Director::getInstance()->getVisibleSize());
    this->setPosition(Vec2::ZERO);
    
        // status bar init
    auto status_position = ui::RelativeLayoutParameter::create();
    status_position->setAlign(ui::RelativeLayoutParameter::RelativeAlign::PARENT_TOP_CENTER_HORIZONTAL);
    
    m_pStatusText->setFontName("fonts/kenvector_future.ttf");
    m_pStatusText->setFontSize(24);
    m_pStatusText->setString("Default status");
    m_pStatusText->setLayoutParameter(status_position);
    this->addChild(m_pStatusText);
    
        // hero pick init
    auto hero_pick_pos = ui::RelativeLayoutParameter::create();
    hero_pick_pos->setAlign(ui::RelativeLayoutParameter::RelativeAlign::PARENT_TOP_CENTER_HORIZONTAL);
    m_pHeroPick->setLayoutParameter(hero_pick_pos);
    this->addChild(m_pHeroPick);
    
        // players list init
    auto players_list_pos = ui::RelativeLayoutParameter::create();
    players_list_pos->setAlign(ui::RelativeLayoutParameter::RelativeAlign::CENTER_IN_PARENT);
    m_pPlayersList->setLayoutParameter(players_list_pos);
    this->addChild(m_pPlayersList);
}

UIHeroPick::UIHeroPick() :
m_nHeroIndex(0)
{
    this->setLayoutType(ui::Layout::Type::RELATIVE);
    this->setPosition(Vec2::ZERO);
    this->setClippingEnabled(false);
//    this->setBackGroundColor(Color3B::GRAY);
//    this->setBackGroundColorType(ui::Layout::BackGroundColorType::SOLID);
    
    auto vis_size = Director::getInstance()->getVisibleSize();
    vis_size.height *= 0.3;
    this->setContentSize(vis_size);
    
        // selected hero text
    auto sel_hero_text = ui::RelativeLayoutParameter::create();
    sel_hero_text->setAlign(ui::RelativeLayoutParameter::RelativeAlign::PARENT_BOTTOM_CENTER_HORIZONTAL);
    
    m_pSelectedHeroText = ui::Text::create(HeroNames[m_nHeroIndex],
                                           "fonts/kenvector_future.ttf",
                                           16);
    m_pSelectedHeroText->setLayoutParameter(sel_hero_text);
    this->addChild(m_pSelectedHeroText);
    
        // selected hero image
    auto sel_hero_image = ui::RelativeLayoutParameter::create();
    sel_hero_image->setAlign(ui::RelativeLayoutParameter::RelativeAlign::CENTER_IN_PARENT);
    
    m_pSelectedHeroImage = ui::ImageView::create(HeroIcons[m_nHeroIndex]);
    m_pSelectedHeroImage->setLayoutParameter(sel_hero_image);
    this->addChild(m_pSelectedHeroImage);
    
        // right button herochange init
    auto right_button_pos = ui::RelativeLayoutParameter::create();
    right_button_pos->setAlign(ui::RelativeLayoutParameter::RelativeAlign::PARENT_RIGHT_CENTER_VERTICAL);
    
    m_pRightChange = ui::Button::create("res/arrow_right.png");
    m_pRightChange->setLayoutParameter(right_button_pos);
    m_pRightChange->addTouchEventListener([this](Ref * pSender, ui::Widget::TouchEventType type)
                                          {
                                              if(type == ui::Widget::TouchEventType::ENDED)
                                              {
                                                  if(m_nHeroIndex != Hero::Type::RANDOM)
                                                  {
                                                      ++m_nHeroIndex;
                                                      m_pSelectedHeroImage->loadTexture(HeroIcons[m_nHeroIndex]);
                                                      m_pSelectedHeroText->setString(HeroNames[m_nHeroIndex]);
                                                  }
                                              }
                                          });
    this->addChild(m_pRightChange);
    
        // left button herochange init
    auto left_button_pos = ui::RelativeLayoutParameter::create();
    left_button_pos->setAlign(ui::RelativeLayoutParameter::RelativeAlign::PARENT_LEFT_CENTER_VERTICAL);
    
    m_pLeftChange = ui::Button::create("res/arrow_left.png");
    m_pLeftChange->setLayoutParameter(left_button_pos);
    m_pLeftChange->addTouchEventListener([this](Ref * pSender, ui::Widget::TouchEventType type)
                                         {
                                             if(type == ui::Widget::TouchEventType::ENDED)
                                             {
                                                 if(m_nHeroIndex != Hero::Type::NO_HERO)
                                                 {
                                                     --m_nHeroIndex;
                                                     m_pSelectedHeroImage->loadTexture(HeroIcons[m_nHeroIndex]);
                                                     m_pSelectedHeroText->setString(HeroNames[m_nHeroIndex]);
                                                 }
                                             }
                                         });
    this->addChild(m_pLeftChange);
}

UIPlayerInfo::UIPlayerInfo()
{
    this->setLayoutType(ui::Layout::Type::HORIZONTAL);
    this->setPosition(Vec2::ZERO);
    
    auto vis_size = Director::getInstance()->getVisibleSize();
    vis_size.height *= 0.125;
    this->setContentSize(vis_size);
    
    auto image_param = ui::LinearLayoutParameter::create();
    image_param->setGravity(ui::LinearLayoutParameter::LinearGravity::CENTER_VERTICAL);
    
    m_pHeroImage = ui::ImageView::create("res/no_hero.png");
    m_pHeroImage->setLayoutParameter(image_param);
    this->addChild(m_pHeroImage);
    
    auto text_param = ui::LinearLayoutParameter::create();
    text_param->setGravity(ui::LinearLayoutParameter::LinearGravity::CENTER_VERTICAL);
    text_param->setMargin(ui::Margin(20, 0, 0, 0));
    
    m_pPlayerName = ui::Text::create("unknown",
                                     "fonts/kenvector_future.ttf",
                                     26);
    m_pPlayerName->setLayoutParameter(text_param);
    this->addChild(m_pPlayerName);
    
    auto ready_param = ui::LinearLayoutParameter::create();
    ready_param->setGravity(ui::LinearLayoutParameter::LinearGravity::CENTER_VERTICAL);
    ready_param->setMargin(ui::Margin(20, 0, 0, 0));
    
    m_pReadyStatus = ui::CheckBox::create("res/no_hero.png",
                                          "res/air_elem.png");
    m_pReadyStatus->setLayoutParameter(ready_param);
    this->addChild(m_pReadyStatus);
}

UIPlayersList::UIPlayersList() :
m_pListView(ui::ListView::create())
{
    auto vis_size = Director::getInstance()->getVisibleSize();
    vis_size.height *= 0.3;
    this->setContentSize(vis_size);
    
    m_pListView = ui::ListView::create();
    m_pListView->setDirection(ui::ScrollView::Direction::VERTICAL);
    m_pListView->setClippingEnabled(true);
    m_pListView->setPosition(Vec2::ZERO);
    m_pListView->setBackGroundColorType(ui::Layout::BackGroundColorType::SOLID);
    m_pListView->setBackGroundColor(Color3B::GREEN);
    m_pListView->setContentSize(vis_size);
    this->addChild(m_pListView);
    
    ui::Text * bar = ui::Text::create("Players list",
                                      "fonts/kenvector_future.ttf", 20);
    m_pListView->pushBackCustomItem(bar);
}

void
UIPlayersList::AddPlayer(const PlayerInfo& player)
{
    UIPlayerInfo * pPlayer = new UIPlayerInfo();
    pPlayer->m_pPlayerName->setString(StringUtils::format("%s#%d",
                                                          player.sNickname.c_str(),
                                                          player.nUID));
    m_pListView->pushBackCustomItem(pPlayer);
}

void
UIPlayersList::RemovePlayer(uint32_t uid)
{
        // TODO: remove required UIPlayerInfo
}
