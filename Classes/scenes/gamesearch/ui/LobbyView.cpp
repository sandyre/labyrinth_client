//
//  LobbyView.cpp
//  labyrinth
//
//  Created by Alexandr Borzykh on 13/09/2017.
//
//

#include "scenes/gamesearch/ui/LobbyView.hpp"

#include "game/client/units/hero.hpp"
#include "resources.hpp"

#include "toolkit/make_autorelease.hpp"

using cocos2d::Color3B;
using cocos2d::Color4B;
using cocos2d::Director;
using cocos2d::Vec2;
using cocos2d::ui::Button;
using cocos2d::ui::CheckBox;
using cocos2d::ui::ImageView;
using cocos2d::ui::Layout;
using cocos2d::ui::ListView;
using cocos2d::ui::Margin;
using cocos2d::ui::PageView;
using cocos2d::ui::RelativeAlign;
using cocos2d::ui::RelativeLayoutParameter;
using cocos2d::ui::ScrollView;
using cocos2d::ui::Text;
using cocos2d::ui::Widget;


namespace labyrinth {
namespace ui {
namespace impl
{

    namespace impl
    {

        bool HeroCarousel::init()
        {
            if (!Layout::init())
                return false;

            const auto visibleSize = Director::getInstance()->getVisibleSize();
            const auto contentSize = cocos2d::Size(visibleSize.width, visibleSize.height * 0.4375);

            this->setCascadeOpacityEnabled(true);
            this->setLayoutType(Layout::Type::RELATIVE);
            this->setContentSize(contentSize);

            auto herosPos = RelativeLayoutParameter::create();
            herosPos->setAlign(RelativeAlign::CENTER_IN_PARENT);

            _heros = PageView::create();
            _heros->setLayoutParameter(herosPos);
            _heros->setDirection(ScrollView::Direction::HORIZONTAL);
            _heros->setContentSize(contentSize);
            _heros->setTouchEnabled(false);
            this->addChild(_heros);

            auto rightButtonPos = RelativeLayoutParameter::create();
            rightButtonPos->setAlign(RelativeAlign::PARENT_RIGHT_CENTER_VERTICAL);

            _rightChangeButton = Button::create("res/ui/ui_buttons/b_vert_m_2.png", "res/ui/ui_buttons/b_vert_m_2_press.png");
            _rightChangeButton->setLayoutParameter(rightButtonPos);
            this->addChild(_rightChangeButton);

            auto leftButtonPos = RelativeLayoutParameter::create();
            leftButtonPos->setAlign(RelativeAlign::PARENT_LEFT_CENTER_VERTICAL);

            _leftChangeButton = Button::create("res/ui/ui_buttons/b_vert_m_1.png", "res/ui/ui_buttons/b_vert_m_1_press.png");
            _leftChangeButton->setLayoutParameter(leftButtonPos);
            this->addChild(_leftChangeButton);

            return true;
        }


        bool PlayerRow::init()
        {
            if (!Layout::init())
                return false;

            const auto visibleSize = Director::getInstance()->getVisibleSize();
            const auto contentSize = cocos2d::Size(visibleSize.width, visibleSize.height * 0.094);

            this->setCascadeOpacityEnabled(true);
            this->setLayoutType(Layout::Type::RELATIVE);
            this->setBackGroundImage("res/ui/ui_plates/p_playerItem.png");
            this->setContentSize(contentSize);

            auto iconPos = RelativeLayoutParameter::create();
            iconPos->setAlign(RelativeAlign::PARENT_LEFT_CENTER_VERTICAL);

            _icon = ImageView::create(HeroIcons[0]);
            _icon->setLayoutParameter(iconPos);
            this->addChild(_icon);

            auto namePos = RelativeLayoutParameter::create();
            namePos->setAlign(RelativeAlign::CENTER_IN_PARENT);

            _name = Text::create(_rawName + "#" + std::to_string(_uuid), TitleFont, 30);
            _name->setLayoutParameter(namePos);
            this->addChild(_name);

            auto statusPos = RelativeLayoutParameter::create();
            statusPos->setAlign(RelativeAlign::PARENT_RIGHT_CENTER_VERTICAL);

            _status = CheckBox::create("res/ui/ui_buttons/b_rect_m_3.png", "res/ui/ui_buttons/b_rect_m_1.png");
            _status->setLayoutParameter(statusPos);
            _status->addTouchEventListener([&](Ref * pSender, Widget::TouchEventType type)
            {
                if (type == Widget::TouchEventType::ENDED && _status->isSelected())
                {
                    _status->setEnabled(false);
                    _status->setTouchEnabled(false);
                    _onReadyClicked();
                }
            });
            _status->setEnabled(false);
            this->addChild(_status);

            return true;
        }


        bool PlayersList::init()
        {
            if (!Layout::init())
                return false;

            const auto visibleSize = Director::getInstance()->getVisibleSize();
            const auto contentSize = cocos2d::Size(visibleSize.width, visibleSize.height * 0.46875);

            this->setCascadeOpacityEnabled(true);
            this->setLayoutType(Layout::Type::RELATIVE);
            this->setBackGroundImage("res/ui/ui_plates/p_playersList.png");
            this->setContentSize(contentSize);

            auto playersLabelPos = RelativeLayoutParameter::create();
            playersLabelPos->setAlign(RelativeAlign::PARENT_TOP_CENTER_HORIZONTAL);
            playersLabelPos->setMargin(Margin(0, 30, 0, 0));

            _playersLabel = Text::create("Players list", TitleFont, 38);
            _playersLabel->setLayoutParameter(playersLabelPos);
            this->addChild(_playersLabel);

            const auto playersListSize = cocos2d::Size(contentSize.width, contentSize.height - visibleSize.height * 0.094);

            auto playersListPos = RelativeLayoutParameter::create();
            playersListPos->setAlign(RelativeAlign::PARENT_BOTTOM_CENTER_HORIZONTAL);

            _playersList = ListView::create();
            _playersList->setLayoutParameter(playersListPos);
            _playersList->setContentSize(playersListSize);
            _playersList->setDirection(ScrollView::Direction::VERTICAL);
            this->addChild(_playersList);

            return true;
        }


        PlayerRow * PlayersList::GetPlayerRow(uint32_t uuid) const
        {
            for (auto player : _playersListInternal)
                if (player->GetUuid() == uuid)
                    return player;

            return nullptr;
        }


        PlayerRow * PlayersList::InsertPlayer(const std::string& name, uint32_t uuid)
        {
            auto playerRow = make_autorelease<PlayerRow>(name, uuid);

            _playersList->pushBackCustomItem(playerRow);
            _playersListInternal.push_back(playerRow);

            return playerRow;
        }


        void PlayersList::RemovePlayer(uint32_t uuid)
        {
            if (const auto playerRow = GetPlayerRow(uuid))
                _playersList->removeChild(playerRow);
        }

    }


    LobbyView::LobbyView()
    : _currentState(LobbyView::State::Forming)
    { }


    bool LobbyView::init()
    {
        if (!Layout::init())
            return false;

        const auto visibleSize = Director::getInstance()->getVisibleSize();

        this->setCascadeOpacityEnabled(true);
        this->setLayoutType(Layout::Type::RELATIVE);
        this->setContentSize(visibleSize);

        // NOTE: Status plate is unmanaged (its only for view)
        auto statusPlatePos = RelativeLayoutParameter::create();
        statusPlatePos->setAlign(RelativeAlign::PARENT_TOP_CENTER_HORIZONTAL);

        auto statusPlate = ImageView::create("res/ui/ui_plates/p_lobbyHeader.png");
        statusPlate->setLayoutParameter(statusPlatePos);
        this->addChild(statusPlate);

        auto statusTextPos = RelativeLayoutParameter::create();
        statusTextPos->setAlign(RelativeAlign::PARENT_TOP_CENTER_HORIZONTAL);
        statusTextPos->setMargin(Margin(0, 30, 0, 0));

        _statusText = Text::create("Lobby view", TitleFont, 38);
        _statusText->setLayoutParameter(statusTextPos);
        this->addChild(_statusText);

        auto heroCarouselPos = RelativeLayoutParameter::create();
        heroCarouselPos->setAlign(RelativeAlign::PARENT_TOP_CENTER_HORIZONTAL);
        heroCarouselPos->setMargin(Margin(0, _statusText->getContentSize().height, 0, 0));

        _heroCarousel = make_autorelease<impl::HeroCarousel>();
        _heroCarousel->setLayoutParameter(heroCarouselPos);
        this->addChild(_heroCarousel);

        auto playersListPos = RelativeLayoutParameter::create();
        playersListPos->setAlign(RelativeAlign::PARENT_BOTTOM_CENTER_HORIZONTAL);

        _playersList = make_autorelease<impl::PlayersList>();
        _playersList->setLayoutParameter(playersListPos);
        this->addChild(_playersList);

        return true;
    }


    void LobbyView::SetState(LobbyView::State state)
    {
        switch (state)
        {
        case State::Forming:
            Director::getInstance()->getScheduler()->performFunctionInCocosThread(std::bind(&Text::setString, _statusText, "Waiting others"));
            break;

        case State::Picking:
            Director::getInstance()->getScheduler()->performFunctionInCocosThread(std::bind(&Text::setString, _statusText, "Pick your hero!"));
            break;
        }

        _currentState = state;
    }

}}}
