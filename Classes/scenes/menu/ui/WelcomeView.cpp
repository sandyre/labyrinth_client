//
//  WelcomeView.cpp
//  labyrinth
//
//  Created by Alexandr Borzykh on 14/09/2017.
//
//

#include "scenes/menu/ui/WelcomeView.hpp"

#include "resources.hpp"

using cocos2d::Color4B;
using cocos2d::Director;
using cocos2d::Vec2;
using cocos2d::ui::Button;
using cocos2d::ui::Layout;
using cocos2d::ui::Margin;
using cocos2d::ui::RelativeLayoutParameter;
using cocos2d::ui::Text;
using cocos2d::ui::Widget;


namespace labyrinth {
namespace ui {
namespace impl
{

    bool WelcomeView::init()
    {
        if (!Layout::init())
            return false;

        auto visibleSize = Director::getInstance()->getVisibleSize();

        this->setCascadeOpacityEnabled(true);
        this->setLayoutType(Layout::Type::RELATIVE);
        this->setContentSize(visibleSize);
        this->setPosition(Vec2::ZERO);

        auto titlePos = RelativeLayoutParameter::create();
        titlePos->setAlign(RelativeLayoutParameter::RelativeAlign::PARENT_TOP_CENTER_HORIZONTAL);
        titlePos->setMargin(Margin(0, visibleSize.height * 0.1, 0, 0));

        _labyrinthLabel = Text::create("Labyrinth", TitleFont, 70);
        _labyrinthLabel->setLayoutParameter(titlePos);
        _labyrinthLabel->enableOutline(Color4B::BLACK);
        this->addChild(_labyrinthLabel);

        auto startPos = RelativeLayoutParameter::create();
        startPos->setAlign(RelativeLayoutParameter::RelativeAlign::PARENT_BOTTOM_CENTER_HORIZONTAL);
        startPos->setMargin(Margin(0, 0, 0, visibleSize.height * 0.3125));

        _startButton = Button::create("res/ui/ui_buttons/b_big_1.png",
                                      "res/ui/ui_buttons/b_big_1_press.png");
        _startButton->setLayoutParameter(startPos);
        _startButton->setTitleText("Begin");
        _startButton->setTitleFontName(TitleFont);
        _startButton->setTitleFontSize(50);
        _startButton->setEnabled(true);
        this->addChild(_startButton);

        _startButton->addTouchEventListener([this](Ref * pSender, Widget::TouchEventType type)
                                                   {
                                                       if (type == Widget::TouchEventType::ENDED)
                                                           _onStartClick();
                                                   });

        return true;
    }

}}}
