//
//  ui_mainmenuscene.cpp
//  labyrinth
//
//  Created by Aleksandr Borzikh on 06.04.17.
//
//

#include "ui_mainmenuscene.hpp"

#include "resources.hpp"

USING_NS_CC;

UIMainMenuScene::UIMainMenuScene()
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    
    m_pPageView = ui::PageView::create();
    m_pPageView->setDirection(ui::ScrollView::Direction::HORIZONTAL);
    m_pPageView->setTouchEnabled(false);
    m_pPageView->setContentSize(visibleSize);
    m_pPageView->setPosition(Vec2::ZERO);
    this->addChild(m_pPageView);
    
    m_pStartPage = new UIStartPage();
    m_pPageView->insertPage(m_pStartPage, 0);
    
    m_pLoginPage = new UILoginPage();
    m_pPageView->insertPage(m_pLoginPage, 1);
    
    m_pMainPage = new UIMainMenuPage();
    m_pPageView->insertPage(m_pMainPage, 2);
}

UIStartPage::UIStartPage()
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    
    this->setLayoutType(ui::Layout::Type::RELATIVE);
    this->setContentSize(visibleSize);
    this->setPosition(Vec2::ZERO);
    this->setBackGroundColorType(ui::Layout::BackGroundColorType::SOLID);
    this->setBackGroundColor(Color3B(14, 50, 4));
    
        // init 'title' label
    auto title_pos = ui::RelativeLayoutParameter::create();
    title_pos->setAlign(ui::RelativeLayoutParameter::RelativeAlign::PARENT_TOP_CENTER_HORIZONTAL);
    title_pos->setMargin(ui::Margin(0,
                                    visibleSize.height * 0.1,
                                    0,
                                    0));
    
    m_pGameTitle = ui::Text::create("Labyrinth",
                                    TitleFont,
                                    70);
    m_pGameTitle->setLayoutParameter(title_pos);
    m_pGameTitle->enableOutline(Color4B::BLACK);
    this->addChild(m_pGameTitle);
    
        // init 'start' button
    auto start_button_pos = ui::RelativeLayoutParameter::create();
    start_button_pos->setAlign(ui::RelativeLayoutParameter::RelativeAlign::PARENT_BOTTOM_CENTER_HORIZONTAL);
    start_button_pos->setMargin(ui::Margin(0,
                                           0,
                                           0,
                                           visibleSize.height * 0.3125));
    
    m_pStartButton = ui::Button::create("res/ui/buttons/b_big_1.png",
                                        "res/ui/buttons/b_big_1_press.png");
    m_pStartButton->setLayoutParameter(start_button_pos);
    m_pStartButton->setTitleText("Offline");
    m_pStartButton->setTitleFontName(TitleFont);
    m_pStartButton->setTitleFontSize(50);
    m_pStartButton->setEnabled(false);
    this->addChild(m_pStartButton);
}

UILoginPage::UILoginPage()
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    
    this->setLayoutType(ui::Layout::Type::RELATIVE);
    this->setContentSize(visibleSize);
    this->setPosition(Vec2::ZERO);
    this->setBackGroundColorType(ui::Layout::BackGroundColorType::SOLID);
    this->setBackGroundColor(Color3B(14, 50, 4));
    
        // init 'login page title' label
    auto page_label_pos = ui::RelativeLayoutParameter::create();
    page_label_pos->setAlign(ui::RelativeLayoutParameter::RelativeAlign::PARENT_TOP_CENTER_HORIZONTAL);
    page_label_pos->setMargin(ui::Margin(0,
                                         visibleSize.height * 0.1,
                                         0,
                                         0));
    
    m_pLoginPageLabel = ui::Text::create("Authorization",
                                         TitleFont,
                                         70);
    m_pLoginPageLabel->setLayoutParameter(page_label_pos);
    this->addChild(m_pLoginPageLabel);
    
        // init form
    auto form_size = visibleSize;
    form_size.width *= 0.777;
    form_size.height *= 0.304;
    
    auto field_size = form_size;
    field_size.height *= 0.1282;
    field_size.width *= 0.8928;
    
    auto form_pos = ui::RelativeLayoutParameter::create();
    form_pos->setAlign(ui::RelativeLayoutParameter::RelativeAlign::CENTER_IN_PARENT);
    
    m_pFormLayout = ui::Layout::create();
    m_pFormLayout->setLayoutType(ui::Layout::Type::RELATIVE);
    m_pFormLayout->setLayoutParameter(form_pos);
    m_pFormLayout->setContentSize(form_size);
    m_pFormLayout->setBackGroundImage("res/ui/plates/p_login.png");
    this->addChild(m_pFormLayout);
    
        // init 'mail' label
    auto mail_label_pos = ui::RelativeLayoutParameter::create();
    mail_label_pos->setAlign(ui::RelativeLayoutParameter::RelativeAlign::PARENT_TOP_LEFT);
    mail_label_pos->setMargin(ui::Margin(form_size.width * 0.05357,
                                         form_size.height * 0.1107,
                                         0,
                                         0));
    
    m_pMailLabel = ui::Text::create("Enter mail",
                                    TitleFont,
                                    30);
    m_pMailLabel->setLayoutParameter(mail_label_pos);
    m_pFormLayout->addChild(m_pMailLabel);
    
        // init 'mail' field
    auto mail_field_pos = ui::RelativeLayoutParameter::create();
    mail_field_pos->setAlign(ui::RelativeLayoutParameter::RelativeAlign::PARENT_TOP_LEFT);
    mail_field_pos->setMargin(ui::Margin(form_size.width * 0.05357,
                                         form_size.height * 0.2307,
                                         0,
                                         0));
    
    m_pMailField = ui::TextField::create("example@gmail.com",
                                         TitleFont,
                                         30);
    m_pMailField->setMaxLengthEnabled(true);
    m_pMailField->setMaxLength(30);
    m_pMailField->setCursorChar('_');
    m_pMailField->setCursorEnabled(true);
    m_pMailField->setLayoutParameter(mail_field_pos);
    m_pMailField->setAnchorPoint(Vec2::ZERO);
    m_pMailField->setTextAreaSize(field_size);
    m_pFormLayout->addChild(m_pMailField);
    
        // init 'password' label
    auto password_label_pos = ui::RelativeLayoutParameter::create();
    password_label_pos->setAlign(ui::RelativeLayoutParameter::RelativeAlign::PARENT_TOP_LEFT);
    password_label_pos->setMargin(ui::Margin(form_size.width * 0.05357,
                                             form_size.height * 0.4341,
                                             0,
                                             0));
    
    m_pPasswordLabel = ui::Text::create("Enter password",
                                        TitleFont,
                                        30);
    m_pPasswordLabel->setLayoutParameter(password_label_pos);
    m_pFormLayout->addChild(m_pPasswordLabel);
    
        // init 'password' field
    auto password_field_pos = ui::RelativeLayoutParameter::create();
    password_field_pos->setAlign(ui::RelativeLayoutParameter::RelativeAlign::PARENT_TOP_LEFT);
    password_field_pos->setMargin(ui::Margin(form_size.width * 0.05357,
                                             form_size.height * 0.5641,
                                             0,
                                             0));
    
    m_pPasswordField = ui::TextField::create("somepassword",
                                             TitleFont,
                                             30);
    m_pPasswordField->setMaxLengthEnabled(true);
    m_pPasswordField->setMaxLength(12);
    m_pPasswordField->setCursorChar('_');
    m_pPasswordField->setCursorEnabled(true);
    m_pPasswordField->setPasswordEnabled(true);
    m_pPasswordField->setPasswordStyleText("*");
    m_pPasswordField->setAnchorPoint(Vec2::ZERO);
    m_pPasswordField->setTextAreaSize(field_size);
    m_pPasswordField->setLayoutParameter(password_field_pos);
    m_pFormLayout->addChild(m_pPasswordField);
    
        // init 'log in' button
    auto login_button_pos = ui::RelativeLayoutParameter::create();
    login_button_pos->setAlign(ui::RelativeLayoutParameter::RelativeAlign::PARENT_RIGHT_BOTTOM);
    login_button_pos->setMargin(ui::Margin(0,
                                           0,
                                           form_size.width * 0.0357,
                                           form_size.height * 0.0769));
    
    m_pLogInButton = ui::Button::create("res/ui/buttons/b_small_1.png",
                                        "res/ui/buttons/b_small_1_press.png");
    m_pLogInButton->setLayoutParameter(login_button_pos);
    m_pLogInButton->setTitleText("Log me in");
    m_pLogInButton->setTitleFontName(TitleFont);
    m_pLogInButton->setTitleFontSize(32);
    m_pFormLayout->addChild(m_pLogInButton);
    
        // init 'reg me' button
    auto reg_button_pos = ui::RelativeLayoutParameter::create();
    reg_button_pos->setAlign(ui::RelativeLayoutParameter::RelativeAlign::PARENT_LEFT_BOTTOM);
    reg_button_pos->setMargin(ui::Margin(form_size.width * 0.0357,
                                         0,
                                         0,
                                         form_size.height * 0.0769));
    
    m_pRegButton = ui::Button::create("res/ui/buttons/b_small_2.png",
                                      "res/ui/buttons/b_small_2.png");
    m_pRegButton->setLayoutParameter(reg_button_pos);
    m_pRegButton->setTitleText("Register");
    m_pRegButton->setTitleFontName(TitleFont);
    m_pRegButton->setTitleFontSize(32);
    m_pFormLayout->addChild(m_pRegButton);
}

UIMainMenuPage::UIMainMenuPage()
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    
    this->setLayoutType(ui::Layout::Type::RELATIVE);
    this->setContentSize(visibleSize);
    this->setPosition(Vec2::ZERO);
    
        // init news layout
    auto news_layout_pos = ui::RelativeLayoutParameter::create();
    news_layout_pos->setAlign(ui::RelativeLayoutParameter::RelativeAlign::PARENT_TOP_CENTER_HORIZONTAL);
    
    m_pNewsLayout = new UINewsLayout();
    m_pNewsLayout->setLayoutParameter(news_layout_pos);
    this->addChild(m_pNewsLayout);
    
        // init central buttons layout
    auto centr_layout_size = visibleSize;
    centr_layout_size.width *= 0.55;
    centr_layout_size.height *= 0.219;
    
    auto centr_layout_pos = ui::RelativeLayoutParameter::create();
    centr_layout_pos->setAlign(ui::RelativeLayoutParameter::RelativeAlign::PARENT_BOTTOM_CENTER_HORIZONTAL);
    centr_layout_pos->setMargin(ui::Margin(0,
                                           0,
                                           0,
                                           visibleSize.height * 0.28));
    
    m_pButtonsLayout = ui::Layout::create();
    m_pButtonsLayout->setLayoutType(ui::Layout::Type::VERTICAL);
    m_pButtonsLayout->setContentSize(centr_layout_size);
    m_pButtonsLayout->setLayoutParameter(centr_layout_pos);
    this->addChild(m_pButtonsLayout);
    
        // init 'play' button
    auto play_but_pos = ui::LinearLayoutParameter::create();
    play_but_pos->setGravity(ui::LinearLayoutParameter::LinearGravity::CENTER_HORIZONTAL);
    
    m_pPlayButton = ui::Button::create("res/ui/buttons/b_mid_1.png",
                                       "res/ui/buttons/b_mid_1_press.png");
    m_pPlayButton->setTitleText("Play");
    m_pPlayButton->setTitleFontName(TitleFont);
    m_pPlayButton->setTitleFontSize(50);
    m_pPlayButton->setLayoutParameter(play_but_pos);
    m_pButtonsLayout->addChild(m_pPlayButton);
    
        // init 'collection' button
    auto collection_but_pos = ui::LinearLayoutParameter::create();
    collection_but_pos->setGravity(ui::LinearLayoutParameter::LinearGravity::CENTER_HORIZONTAL);
    collection_but_pos->setMargin(ui::Margin(0,
                                             visibleSize.height * 0.031,
                                             0,
                                             0));
    
    m_pCollectionButton = ui::Button::create("res/ui/buttons/b_mid_2.png",
                                             "res/ui/buttons/b_mid_2_press.png");
    m_pCollectionButton->setTitleText("Collection");
    m_pCollectionButton->setTitleFontName(TitleFont);
    m_pCollectionButton->setTitleFontSize(50);
    m_pCollectionButton->setLayoutParameter(collection_but_pos);
    m_pButtonsLayout->addChild(m_pCollectionButton);
    
        // init settings button
    auto sett_but_pos = ui::RelativeLayoutParameter::create();
    sett_but_pos->setAlign(ui::RelativeLayoutParameter::RelativeAlign::PARENT_RIGHT_BOTTOM);
    sett_but_pos->setMargin(ui::Margin(0,
                                       0,
                                       visibleSize.width * 0.15,
                                       visibleSize.height * 0.09));
    
    m_pSettingsButton = ui::Button::create("res/ui/buttons/b_rect_b_2.png",
                                           "res/ui/buttons/b_rect_b_2_press.png");
    m_pSettingsButton->setLayoutParameter(sett_but_pos);
    this->addChild(m_pSettingsButton);
    
        // init donate button
    auto shop_but_pos = ui::RelativeLayoutParameter::create();
    shop_but_pos->setAlign(ui::RelativeLayoutParameter::RelativeAlign::PARENT_LEFT_BOTTOM);
    shop_but_pos->setMargin(ui::Margin(visibleSize.width * 0.15,
                                       0,
                                       0,
                                       visibleSize.height * 0.09));
    
    m_pShopButton = ui::Button::create("res/ui/buttons/b_rect_b_1.png",
                                       "res/ui/buttons/b_rect_b_1_press.png");
    m_pShopButton->setLayoutParameter(shop_but_pos);
    this->addChild(m_pShopButton);
}

UINewsLayout::UINewsLayout()
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    
    auto layout_size = visibleSize;
    layout_size.height *= 0.375;
    
    this->setLayoutType(ui::Layout::Type::RELATIVE);
    this->setBackGroundImage("res/ui/plates/p_news.png");
    this->setContentSize(layout_size);
    this->setPosition(Vec2::ZERO);
    
        // init News label
    auto news_label_pos = ui::RelativeLayoutParameter::create();
    news_label_pos->setAlign(ui::RelativeLayoutParameter::RelativeAlign::PARENT_TOP_LEFT);
    news_label_pos->setMargin(ui::Margin(10,
                                         20,
                                         0,
                                         0)); // FIXME: hardcoded
    
    m_pNewsLabel = ui::Text::create("Newsfeed",
                                    TitleFont,
                                    36);
    m_pNewsLabel->setLayoutParameter(news_label_pos);
    this->addChild(m_pNewsLabel);
    
        // init reload button
    auto reload_button_pos = ui::RelativeLayoutParameter::create();
    reload_button_pos->setAlign(ui::RelativeLayoutParameter::RelativeAlign::PARENT_TOP_RIGHT);
    
    m_pReloadButton = ui::Button::create("res/ui/buttons/b_rect_s_1.png",
                                         "res/ui/buttons/b_rect_s_1_press.png");
    m_pReloadButton->setLayoutParameter(reload_button_pos);
    this->addChild(m_pReloadButton);
    
        // init news listview
    auto listview_size = layout_size;
    listview_size.height *= 0.82;
    
    auto news_list_pos = ui::RelativeLayoutParameter::create();
    news_list_pos->setAlign(ui::RelativeLayoutParameter::RelativeAlign::PARENT_LEFT_BOTTOM);
    news_list_pos->setMargin(ui::Margin(10,
                                        0,
                                        0,
                                        0));
    
    m_pNewsListView = ui::ListView::create();
    m_pNewsListView->setDirection(ui::ScrollView::Direction::VERTICAL);
    m_pNewsListView->setPosition(Vec2::ZERO);
    m_pNewsListView->setContentSize(listview_size);
    m_pNewsListView->setLayoutParameter(news_list_pos);
    this->addChild(m_pNewsListView);
}
