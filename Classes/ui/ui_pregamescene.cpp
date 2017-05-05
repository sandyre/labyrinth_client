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
    auto status_bar_pos = ui::RelativeLayoutParameter::create();
    status_bar_pos->setAlign(ui::RelativeLayoutParameter::RelativeAlign::PARENT_TOP_CENTER_HORIZONTAL);
    
    auto status_bar = ui::ImageView::create("res/ui/plates/p_lobbyHeader.png");
    status_bar->setLayoutParameter(status_bar_pos);
    this->addChild(status_bar);
    
        // status text init
    auto status_text_pos = ui::RelativeLayoutParameter::create();
    status_text_pos->setAlign(ui::RelativeLayoutParameter::RelativeAlign::PARENT_TOP_CENTER_HORIZONTAL);
    status_text_pos->setMargin(ui::Margin(0, 30, 0, 0)); // FIXME: remove hardcode
    
    m_pStatusText = ui::Text::create("Default status",
                                     TitleFont,
                                     38);
    m_pStatusText->setLayoutParameter(status_text_pos);
    this->addChild(m_pStatusText);
    
        // hero pick init
    auto hero_pick_pos = ui::RelativeLayoutParameter::create();
    hero_pick_pos->setAlign(ui::RelativeLayoutParameter::RelativeAlign::PARENT_TOP_CENTER_HORIZONTAL);
    hero_pick_pos->setMargin(ui::Margin(0, status_bar->getContentSize().height, 0, 0));
    
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
    
    auto vis_size = Director::getInstance()->getVisibleSize();
    vis_size.height *= 0.4375;
    this->setContentSize(vis_size);
    
        // init hero page view
    auto hero_view_pos = ui::RelativeLayoutParameter::create();
    hero_view_pos->setAlign(ui::RelativeLayoutParameter::RelativeAlign::CENTER_IN_PARENT);
    
    m_pHeroPageView = ui::PageView::create();
    m_pHeroPageView->setDirection(ui::ScrollView::Direction::HORIZONTAL);
    m_pHeroPageView->setLayoutParameter(hero_view_pos);
    m_pHeroPageView->setContentSize(vis_size);
    m_pHeroPageView->setTouchEnabled(false);
    this->addChild(m_pHeroPageView);
    
        // init heros
    {
        for(int i = Hero::FIRST_HERO; i <= Hero::LAST_HERO; ++i)
        {
            auto hero_layout = ui::Layout::create();
            hero_layout->setLayoutType(ui::Layout::Type::RELATIVE);
            
            auto hero_img_pos = ui::RelativeLayoutParameter::create();
            hero_img_pos->setAlign(ui::RelativeLayoutParameter::RelativeAlign::CENTER_IN_PARENT);
            
            auto hero_img = ui::ImageView::create(HeroIcons[i]);
            hero_img->setLayoutParameter(hero_img_pos);
            hero_layout->addChild(hero_img);
            
            auto hero_name_pos = ui::RelativeLayoutParameter::create();
            hero_name_pos->setAlign(ui::RelativeLayoutParameter::RelativeAlign::PARENT_TOP_CENTER_HORIZONTAL);
            
            auto hero_name = ui::Text::create(HeroNames[i],
                                              TitleFont,
                                              30);
            hero_name->setLayoutParameter(hero_name_pos);
            
            hero_layout->addChild(hero_name);
            
            m_pHeroPageView->insertPage(hero_layout, i);
        }
    }
    
        // right button herochange init
    auto right_button_pos = ui::RelativeLayoutParameter::create();
    right_button_pos->setAlign(ui::RelativeLayoutParameter::RelativeAlign::PARENT_RIGHT_CENTER_VERTICAL);
    
    m_pRightChange = ui::Button::create("res/ui/buttons/b_vert_m_2.png",
                                        "res/ui/buttons/b_vert_m_2_press.png");
    m_pRightChange->setLayoutParameter(right_button_pos);
    this->addChild(m_pRightChange);
    
        // left button herochange init
    auto left_button_pos = ui::RelativeLayoutParameter::create();
    left_button_pos->setAlign(ui::RelativeLayoutParameter::RelativeAlign::PARENT_LEFT_CENTER_VERTICAL);
    
    m_pLeftChange = ui::Button::create("res/ui/buttons/b_vert_m_1.png",
                                       "res/ui/buttons/b_vert_m_1_press.png");
    m_pLeftChange->setLayoutParameter(left_button_pos);
    this->addChild(m_pLeftChange);
}

UIPlayerInfo::UIPlayerInfo()
{
    this->setLayoutType(ui::Layout::Type::RELATIVE);
    this->setPosition(Vec2::ZERO);
    this->setBackGroundImage("res/ui/plates/p_playerItem.png");
    
    auto vis_size = Director::getInstance()->getVisibleSize();
    vis_size.height *= 0.094;
    this->setContentSize(vis_size);
    
        // init hero icon
    auto image_param = ui::RelativeLayoutParameter::create();
    image_param->setAlign(ui::RelativeLayoutParameter::RelativeAlign::PARENT_LEFT_CENTER_VERTICAL);
    
    m_pHeroImage = ui::ImageView::create(HeroIcons[0]);
    m_pHeroImage->setLayoutParameter(image_param);
    this->addChild(m_pHeroImage);
    
        // init players name
    auto text_param = ui::RelativeLayoutParameter::create();
    text_param->setAlign(ui::RelativeLayoutParameter::RelativeAlign::CENTER_IN_PARENT);
    
    m_pPlayerName = ui::Text::create("unknown",
                                     TitleFont,
                                     30);
    m_pPlayerName->setLayoutParameter(text_param);
    this->addChild(m_pPlayerName);
    
        // init 'ready' checkbox
    auto ready_param = ui::RelativeLayoutParameter::create();
    ready_param->setAlign(ui::RelativeLayoutParameter::RelativeAlign::PARENT_RIGHT_CENTER_VERTICAL);
    
    m_pReadyStatus = ui::CheckBox::create("res/ui/buttons/b_rect_m_3.png",
                                          "res/ui/buttons/b_rect_m_1.png");
    m_pReadyStatus->setLayoutParameter(ready_param);
    m_pReadyStatus->setEnabled(false);
    this->addChild(m_pReadyStatus);
}

UIPlayersList::UIPlayersList()
{
    this->setLayoutType(ui::Layout::Type::RELATIVE);
    this->setBackGroundImage("res/ui/plates/p_playersList.png");
    
    auto vis_size = Director::getInstance()->getVisibleSize();
    vis_size.height *= 0.46875;
    this->setContentSize(vis_size);
    
        // init 'players' label
    auto players_label_pos = ui::RelativeLayoutParameter::create();
    players_label_pos->setAlign(ui::RelativeLayoutParameter::RelativeAlign::PARENT_TOP_CENTER_HORIZONTAL);
    players_label_pos->setMargin(ui::Margin(0, 30, 0, 0));
    
    m_pPlayersLabel = ui::Text::create("Players list",
                                       TitleFont,
                                       38);
    m_pPlayersLabel->setLayoutParameter(players_label_pos);
    this->addChild(m_pPlayersLabel);
    
        // init players list view
    auto list_size = vis_size;
    list_size.height -= (Director::getInstance()->getVisibleSize().height * 0.094);
    
    auto players_list_pos = ui::RelativeLayoutParameter::create();
    players_list_pos->setAlign(ui::RelativeLayoutParameter::RelativeAlign::PARENT_BOTTOM_CENTER_HORIZONTAL);
    
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
