//
//  ui_GameSearch.hpp
//  labyrinth
//
//  Created by Alexandr Borzykh on 13/09/2017.
//
//

#include "scenes/gamesearch/ui/ui_GameSearch.hpp"

#include "toolkit/make_autorelease.hpp"

using cocos2d::Vec2;
using cocos2d::ui::Text;
using cocos2d::ui::RelativeAlign;
using cocos2d::ui::RelativeLayoutParameter;


namespace labyrinth {
namespace ui
{

    GameSearch::GameSearch()
    : _currentView(View::Search)
    { }


    bool GameSearch::init()
    {
        if (!Layout::init())
            return false;

        const auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();

        this->setLayoutType(Layout::Type::RELATIVE);
        this->setContentSize(visibleSize);

        auto searchViewPos = RelativeLayoutParameter::create();
        searchViewPos->setAlign(RelativeAlign::CENTER_IN_PARENT);

        _searchView = make_autorelease<impl::SearchView>();
        _searchView->setLayoutParameter(searchViewPos);
        this->addChild(_searchView);

        auto lobbyViewPos = RelativeLayoutParameter::create();
        lobbyViewPos->setAlign(RelativeAlign::CENTER_IN_PARENT);

        _lobbyView = make_autorelease<impl::LobbyView>();
        _lobbyView->setLayoutParameter(lobbyViewPos);
        _lobbyView->setOpacity(0);
        _lobbyView->setEnabled(false);
        this->addChild(_lobbyView);

        auto loadingViewPos = RelativeLayoutParameter::create();
        loadingViewPos->setAlign(RelativeAlign::CENTER_IN_PARENT);

        _loadingView = make_autorelease<impl::LoadingView>();
        _loadingView->setLayoutParameter(loadingViewPos);
        _loadingView->setOpacity(0);
        _loadingView->setEnabled(false);
        this->addChild(_loadingView);

        return true;
    }


    void GameSearch::SwitchTo(GameSearch::View view)
    {
        switch (_currentView)
        {
        case View::Search:
            _searchView->runAction(cocos2d::FadeOut::create(1.0));
            _searchView->setEnabled(false);
            break;

        case View::Lobby:
            _lobbyView->runAction(cocos2d::FadeOut::create(1.0));
            _lobbyView->setEnabled(false);
            break;

        case View::Loading:
            _loadingView->runAction(cocos2d::FadeOut::create(1.0));
            _loadingView->setEnabled(false);
            break;
        }

        switch (view)
        {
        case View::Search:
            _searchView->runAction(cocos2d::FadeIn::create(1.0));
            _searchView->setEnabled(true);
            break;

        case View::Lobby:
            _lobbyView->runAction(cocos2d::FadeIn::create(1.0));
            _lobbyView->setEnabled(true);
            break;

        case View::Loading:
            _loadingView->runAction(cocos2d::FadeIn::create(1.0));
            _loadingView->setEnabled(true);
            break;
        }

        _currentView = view;
    }

}}
