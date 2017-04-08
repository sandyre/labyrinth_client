//
//  ui_mainmenuscene.cpp
//  labyrinth
//
//  Created by Aleksandr Borzikh on 06.04.17.
//
//

#include "ui_mainmenuscene.hpp"

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
    this->setBackGroundColor(Color3B(100, 68, 53));
    
        // init 'title' label
    auto title_pos = ui::RelativeLayoutParameter::create();
    title_pos->setAlign(ui::RelativeLayoutParameter::RelativeAlign::PARENT_TOP_CENTER_HORIZONTAL);
    
    m_pGameTitle = ui::Text::create("Labyrinth",
                                    "fonts/jigsaw trouserdrop.ttf",
                                    40);
    m_pGameTitle->setLayoutParameter(title_pos);
    this->addChild(m_pGameTitle);
    
        // init 'start' button
    auto start_button_pos = ui::RelativeLayoutParameter::create();
    start_button_pos->setAlign(ui::RelativeLayoutParameter::RelativeAlign::CENTER_IN_PARENT);
    
    m_pStartButton = ui::Button::create("res/button_normal.png",
                                        "res/button_pressed.png");
    m_pStartButton->setLayoutParameter(start_button_pos);
    m_pStartButton->setTitleText("Begin");
    m_pStartButton->setTitleFontName("fonts/kenvector_future.ttf");
    m_pStartButton->setTitleFontSize(24);
    this->addChild(m_pStartButton);
}

UILoginPage::UILoginPage()
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    
    this->setLayoutType(ui::Layout::Type::RELATIVE);
    this->setContentSize(visibleSize);
    this->setPosition(Vec2::ZERO);
    this->setBackGroundColorType(ui::Layout::BackGroundColorType::SOLID);
    this->setBackGroundColor(Color3B(100, 68, 53));
    
        // init 'login page title' label
    auto page_label_pos = ui::RelativeLayoutParameter::create();
    page_label_pos->setAlign(ui::RelativeLayoutParameter::RelativeAlign::PARENT_TOP_CENTER_HORIZONTAL);
    
    m_pLoginPageLabel = ui::Text::create("Authorization",
                                         "fonts/jigsaw trouserdrop.ttf",
                                         40);
    m_pLoginPageLabel->setLayoutParameter(page_label_pos);
    this->addChild(m_pLoginPageLabel);
    
        // init 'log in' button
    auto login_button_pos = ui::RelativeLayoutParameter::create();
    login_button_pos->setAlign(ui::RelativeLayoutParameter::RelativeAlign::CENTER_IN_PARENT);
    
    m_pLogInButton = ui::Button::create("res/button_normal.png",
                                        "res/button_pressed.png");
    m_pLogInButton->setLayoutParameter(login_button_pos);
    m_pLogInButton->setTitleText("Log me in");
    m_pLogInButton->setTitleFontName("fonts/kenvector_future.ttf");
    m_pLogInButton->setTitleFontSize(24);
    this->addChild(m_pLogInButton);
}

UIMainMenuPage::UIMainMenuPage()
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    
    this->setLayoutType(ui::Layout::Type::RELATIVE);
    this->setContentSize(visibleSize);
    this->setPosition(Vec2::ZERO);
    this->setBackGroundColorType(ui::Layout::BackGroundColorType::SOLID);
    this->setBackGroundColor(Color3B(37, 68, 53));
    
        // init gameinfo label
    auto gameinfo_pos = ui::RelativeLayoutParameter::create();
    gameinfo_pos->setAlign(ui::RelativeLayoutParameter::RelativeAlign::PARENT_TOP_RIGHT);
    
    m_pGameInfo = ui::Text::create(StringUtils::format("%d.%d.%d\n",
                                                       GAMEVERSION_MAJOR,
                                                       GAMEVERSION_MINOR,
                                                       GAMEVERSION_BUILD),
                                   "fonts/kenvector_future.ttf",
                                   16);
    m_pGameInfo->setLayoutParameter(gameinfo_pos);
    this->addChild(m_pGameInfo);
    
        // init news layout
    auto news_layout_pos = ui::RelativeLayoutParameter::create();
    news_layout_pos->setAlign(ui::RelativeLayoutParameter::RelativeAlign::PARENT_TOP_CENTER_HORIZONTAL);
    
    m_pNewsLayout = new UINewsLayout();
    m_pNewsLayout->setLayoutParameter(news_layout_pos);
    this->addChild(m_pNewsLayout);
    
        // init central buttons layout
    auto centr_layout_size = visibleSize;
    centr_layout_size.height *= 0.33;
    
    auto centr_layout_pos = ui::RelativeLayoutParameter::create();
    centr_layout_pos->setAlign(ui::RelativeLayoutParameter::RelativeAlign::CENTER_IN_PARENT);
    
    m_pButtonsLayout = ui::Layout::create();
    m_pButtonsLayout->setLayoutType(ui::Layout::Type::VERTICAL);
    m_pButtonsLayout->setContentSize(centr_layout_size);
    m_pButtonsLayout->setLayoutParameter(centr_layout_pos);
    m_pButtonsLayout->setBackGroundColorType(ui::Layout::BackGroundColorType::SOLID);
    m_pButtonsLayout->setBackGroundColor(Color3B(100, 100, 150));
    this->addChild(m_pButtonsLayout);
    
        // init 'play' button
    auto play_but_pos = ui::LinearLayoutParameter::create();
    play_but_pos->setGravity(ui::LinearLayoutParameter::LinearGravity::CENTER_HORIZONTAL);
    
    m_pPlayButton = ui::Button::create("res/button_normal.png",
                                       "res/button_pressed.png");
    m_pPlayButton->setTitleText("Play");
    m_pPlayButton->setTitleFontName("fonts/kenvector_future.ttf");
    m_pPlayButton->setTitleFontSize(16);
    m_pPlayButton->setLayoutParameter(play_but_pos);
    m_pButtonsLayout->addChild(m_pPlayButton);
    
        // init 'collection' button
    auto collection_but_pos = ui::LinearLayoutParameter::create();
    collection_but_pos->setGravity(ui::LinearLayoutParameter::LinearGravity::CENTER_HORIZONTAL);
    
    m_pCollectionButton = ui::Button::create("res/button_normal.png",
                                             "res/button_pressed.png");
    m_pCollectionButton->setTitleText("Collection");
    m_pCollectionButton->setTitleFontName("fonts/kenvector_future.ttf");
    m_pCollectionButton->setTitleFontSize(16);
    m_pCollectionButton->setLayoutParameter(collection_but_pos);
    m_pButtonsLayout->addChild(m_pCollectionButton);
    
        // init settings button
    auto sett_but_pos = ui::RelativeLayoutParameter::create();
    sett_but_pos->setAlign(ui::RelativeLayoutParameter::RelativeAlign::PARENT_RIGHT_BOTTOM);
    
    m_pSettingsButton = ui::Button::create("res/settings.png");
    m_pSettingsButton->setTitleText("Settings");
    m_pSettingsButton->setTitleFontName("fonts/kenvector_future.ttf");
    m_pSettingsButton->setTitleFontSize(16);
    m_pSettingsButton->setLayoutParameter(sett_but_pos);
    this->addChild(m_pSettingsButton);
    
        // init donate button
    auto shop_but_pos = ui::RelativeLayoutParameter::create();
    shop_but_pos->setAlign(ui::RelativeLayoutParameter::RelativeAlign::PARENT_LEFT_BOTTOM);
    
    m_pShopButton = ui::Button::create("res/donate.png");
    m_pShopButton->setTitleText("Donate");
    m_pShopButton->setTitleFontName("fonts/kenvector_future.ttf");
    m_pShopButton->setTitleFontSize(16);
    m_pShopButton->setLayoutParameter(shop_but_pos);
    this->addChild(m_pShopButton);
}

UINewsLayout::UINewsLayout()
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    
    auto layout_size = visibleSize;
    layout_size.height *= 0.33;
    
    this->setLayoutType(ui::Layout::Type::RELATIVE);
    this->setContentSize(layout_size);
    this->setPosition(Vec2::ZERO);
    this->setBackGroundColorType(ui::Layout::BackGroundColorType::SOLID);
    this->setBackGroundColor(Color3B(125, 40, 50));
    
        // init News label
    auto news_label_pos = ui::RelativeLayoutParameter::create();
    news_label_pos->setAlign(ui::RelativeLayoutParameter::RelativeAlign::PARENT_TOP_LEFT);
    
    m_pNewsLabel = ui::Text::create(StringUtils::format("News feed (gamever. %d.%d.%d)",
                                                        GAMEVERSION_MAJOR,
                                                        GAMEVERSION_MINOR,
                                                        GAMEVERSION_BUILD),
                                    "fonts/kenvector_future.ttf",
                                    16);
    m_pNewsLabel->setLayoutParameter(news_label_pos);
    this->addChild(m_pNewsLabel);
    
        // init reload button
    auto reload_button_pos = ui::RelativeLayoutParameter::create();
    reload_button_pos->setAlign(ui::RelativeLayoutParameter::RelativeAlign::PARENT_TOP_RIGHT);
    
    m_pReloadButton = ui::Button::create("res/reload_button.png");
    m_pReloadButton->setLayoutParameter(reload_button_pos);
    this->addChild(m_pReloadButton);
    
        // init news listview
    auto listview_size = layout_size;
    listview_size.height *= 0.9;
    
    auto news_list_pos = ui::RelativeLayoutParameter::create();
    news_list_pos->setAlign(ui::RelativeLayoutParameter::RelativeAlign::PARENT_LEFT_BOTTOM);
    
    m_pNewsListView = ui::ListView::create();
    m_pNewsListView->setDirection(ui::ScrollView::Direction::VERTICAL);
    m_pNewsListView->setPosition(Vec2::ZERO);
    m_pNewsListView->setContentSize(listview_size);
    m_pNewsListView->setLayoutParameter(news_list_pos);
    m_pNewsListView->setBackGroundColorType(ui::Layout::BackGroundColorType::SOLID);
    m_pNewsListView->setBackGroundColor(Color3B(170, 100, 50));
    this->addChild(m_pNewsListView);
    
        // init sample news
    for(int i = 0; i < 5; ++i)
    {
        auto sample_text = ui::Text::create(StringUtils::format("Sample text #%d\n"
                                                                "A lot of news over here, with some dates, etc\n"
                                                                "Sometimes maybe pictures?", i),
                                            "fonts/kenvector_future.ttf",
                                            16);
        m_pNewsListView->pushBackCustomItem(sample_text);
        m_pNewsArray.push_back(sample_text);
    }
}
