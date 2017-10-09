//
//  ui_Menu.cpp
//  labyrinth
//
//  Created by Alexandr Borzykh on 14/09/2017.
//
//

#include "scenes/menu/ui/ui_Menu.hpp"

#include "toolkit/make_autorelease.hpp"

using cocos2d::Color4B;
using cocos2d::Director;
using cocos2d::Vec2;
using cocos2d::ui::Button;
using cocos2d::ui::Layout;
using cocos2d::ui::Margin;
using cocos2d::ui::RelativeLayoutParameter;
using cocos2d::ui::Text;


namespace labyrinth {
namespace ui
{

    Menu::Menu()
    : _currentView(Menu::View::Welcome)
    { }


    bool Menu::init()
    {
        if (!Layout::init())
            return false;

        const auto visibleSize = Director::getInstance()->getVisibleSize();

        this->setContentSize(visibleSize);
        this->setPosition(Vec2::ZERO);

        _welcomeView = make_autorelease<ui::impl::WelcomeView>();
        this->addChild(_welcomeView);

        _loginView = make_autorelease<ui::impl::LoginView>();
        _loginView->setOpacity(0);
        _loginView->setEnabled(false);
        this->addChild(_loginView);

        _menuView = make_autorelease<ui::impl::MenuView>();
        _menuView->setOpacity(0);
        _menuView->setEnabled(false);
        this->addChild(_menuView);

        return true;
    }


    void Menu::SwitchTo(Menu::View view)
    {
        switch (_currentView)
        {
        case Menu::View::Welcome:
            _welcomeView->runAction(cocos2d::FadeOut::create(0.2));
            _welcomeView->setEnabled(false);
            break;

        case Menu::View::Login:
            _loginView->runAction(cocos2d::FadeOut::create(0.2));
            _loginView->setEnabled(false);
            break;

        case Menu::View::Main:
            _menuView->runAction(cocos2d::FadeOut::create(0.2));
            _menuView->setEnabled(false);
            break;
        }

        switch (view)
        {
        case Menu::View::Welcome:
            _welcomeView->runAction(cocos2d::FadeIn::create(1.0));
            _welcomeView->setEnabled(true);
            break;

        case Menu::View::Login:
            _loginView->runAction(cocos2d::FadeIn::create(1.0));
            _loginView->setEnabled(true);
            break;

        case Menu::View::Main:
            _menuView->runAction(cocos2d::FadeIn::create(1.0));
            _menuView->setEnabled(true);
            break;
        }

        _currentView = view;
    }

}}
