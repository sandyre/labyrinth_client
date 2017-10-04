//
//  MenuView.cpp
//  labyrinth
//
//  Created by Alexandr Borzykh on 15/09/2017.
//
//

#include "scenes/menu/ui/MenuView.hpp"

#include "resources.hpp"

using cocos2d::Color4B;
using cocos2d::Director;
using cocos2d::Vec2;
using cocos2d::ui::Button;
using cocos2d::ui::Layout;
using cocos2d::ui::LinearGravity;
using cocos2d::ui::LinearLayoutParameter;
using cocos2d::ui::Margin;
using cocos2d::ui::RelativeAlign;
using cocos2d::ui::RelativeLayoutParameter;
using cocos2d::ui::Text;
using cocos2d::ui::TextField;
using cocos2d::ui::Widget;


namespace labyrinth {
namespace ui {
namespace impl
{

    bool MenuView::init()
    {
        if (!Layout::init())
            return false;

        auto visibleSize = Director::getInstance()->getVisibleSize();

        this->setCascadeOpacityEnabled(true);
        this->setLayoutType(Layout::Type::RELATIVE);
        this->setContentSize(visibleSize);
        this->setPosition(Vec2::ZERO);

        auto menuFormSize = cocos2d::Size(visibleSize.width * 0.55, visibleSize.height * 0.219);

        auto menuFormPos = RelativeLayoutParameter::create();
        menuFormPos->setAlign(RelativeAlign::CENTER_IN_PARENT);
        menuFormPos->setMargin(Margin(0, 0, 0, visibleSize.height * 0.28));

        _menuFormLayout = Layout::create();
        _menuFormLayout->setCascadeOpacityEnabled(true);
        _menuFormLayout->setLayoutParameter(menuFormPos);
        _menuFormLayout->setLayoutType(Layout::Type::VERTICAL);
        _menuFormLayout->setContentSize(menuFormSize);
        this->addChild(_menuFormLayout);


        auto searchButtonPos = LinearLayoutParameter::create();
        searchButtonPos->setGravity(LinearGravity::CENTER_HORIZONTAL);

        _searchButton = Button::create("res/ui/ui_buttons/b_mid_1.png", "res/ui/ui_buttons/b_mid_1_press.png");
        _searchButton->setLayoutParameter(searchButtonPos);
        _searchButton->setTitleText("Search");
        _searchButton->setTitleFontName(TitleFont);
        _searchButton->setTitleFontSize(50);

        _searchButton->addTouchEventListener([this](Ref * pSender, Widget::TouchEventType type)
                                             {
                                                 if (type == Widget::TouchEventType::ENDED)
                                                     _onSearchClick();
                                             });

        _menuFormLayout->addChild(_searchButton);


        auto collectionButtonPos = LinearLayoutParameter::create();
        collectionButtonPos->setGravity(LinearGravity::CENTER_HORIZONTAL);
        collectionButtonPos->setMargin(Margin(0, visibleSize.height * 0.031, 0, 0));

        _collectionButton = Button::create("res/ui/ui_buttons/b_mid_2.png", "res/ui/ui_buttons/b_mid_2_press.png");
        _collectionButton->setLayoutParameter(collectionButtonPos);
        _collectionButton->setTitleText("Collection");
        _collectionButton->setTitleFontName(TitleFont);
        _collectionButton->setTitleFontSize(50);
        _menuFormLayout->addChild(_collectionButton);


        auto settingsButtonPos = RelativeLayoutParameter::create();
        settingsButtonPos->setAlign(RelativeAlign::PARENT_RIGHT_BOTTOM);
        settingsButtonPos->setMargin(Margin(0, 0, visibleSize.width * 0.15, visibleSize.height * 0.09));

        _settingsButton = Button::create("res/ui/ui_buttons/b_rect_b_2.png", "res/ui/ui_buttons/b_rect_b_2_press.png");
        _settingsButton->setLayoutParameter(settingsButtonPos);
        this->addChild(_settingsButton);


        auto shopButtonPos = RelativeLayoutParameter::create();
        shopButtonPos->setAlign(RelativeAlign::PARENT_LEFT_BOTTOM);
        shopButtonPos->setMargin(Margin(visibleSize.width * 0.15, 0, 0, visibleSize.height * 0.09));

        _shopButton = Button::create("res/ui/ui_buttons/b_rect_b_1.png", "res/ui/ui_buttons/b_rect_b_1_press.png");
        _shopButton->setLayoutParameter(shopButtonPos);
        this->addChild(_shopButton);

        return true;
    }

}}}
