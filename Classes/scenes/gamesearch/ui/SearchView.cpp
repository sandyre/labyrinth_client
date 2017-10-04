//
//  SearchView.cpp
//  labyrinth
//
//  Created by Alexandr Borzykh on 13/09/2017.
//
//

#include "SearchView.hpp"

#include "resources.hpp"

using cocos2d::Director;
using cocos2d::Vec2;
using cocos2d::ui::Text;
using cocos2d::ui::RelativeLayoutParameter;


namespace labyrinth {
namespace ui {
namespace impl
{

    SearchView::SearchView()
    {
        auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();

        this->setCascadeOpacityEnabled(true);
        this->setLayoutType(Layout::Type::RELATIVE);
        this->setContentSize(visibleSize);
        this->setPosition(Vec2::ZERO);

        auto statusTextPos = RelativeLayoutParameter::create();
        statusTextPos->setAlign(RelativeLayoutParameter::RelativeAlign::PARENT_TOP_CENTER_HORIZONTAL);

        _statusText = Text::create("GameSearch view", TitleFont, 40);
        _statusText->setLayoutParameter(statusTextPos);
        this->addChild(_statusText);

        CCLOG("qoolity");
    }


    void SearchView::SetStatus(const std::string& status)
    {
        Director::getInstance()->getScheduler()->performFunctionInCocosThread(std::bind(&Text::setString, _statusText, status));
    }

}}}
