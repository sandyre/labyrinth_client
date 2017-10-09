//
//  LoadingView.cpp
//  labyrinth
//
//  Created by Alexandr Borzykh on 10/10/2017.
//

#include "LoadingView.hpp"

#include "resources.hpp"

using cocos2d::Director;
using cocos2d::Vec2;
using cocos2d::ui::Text;
using cocos2d::ui::RelativeLayoutParameter;


namespace labyrinth {
namespace ui {
namespace impl
{

    bool LoadingView::init()
    {
        if (!Layout::init())
            return false;

        const auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();

        this->setCascadeOpacityEnabled(true);
        this->setLayoutType(Layout::Type::RELATIVE);
        this->setContentSize(visibleSize);

        auto statusTextPos = RelativeLayoutParameter::create();
        statusTextPos->setAlign(RelativeLayoutParameter::RelativeAlign::CENTER_IN_PARENT);

        _statusText = Text::create("GameSearch view", TitleFont, 40);
        _statusText->setLayoutParameter(statusTextPos);
        this->addChild(_statusText);

        return true;
    }


    void LoadingView::SetStatus(const std::string& status)
    {
        Director::getInstance()->getScheduler()->performFunctionInCocosThread(std::bind(&Text::setString, _statusText, status));
    }

}}}
