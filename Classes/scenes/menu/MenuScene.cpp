//
//  MenuScene.cpp
//  labyrinth
//
//  Created by Alexandr Borzykh on 14/09/2017.
//
//

#include "scenes/menu/MenuScene.hpp"

#include "scenes/gamesearch/GameSearchScene.hpp"

#include "toolkit/make_autorelease.hpp"

using cocos2d::Director;


namespace labyrinth
{

    bool MenuScene::init()
    {
        if (!Scene::init())
            return false;

        auto visibleSize = Director::getInstance()->getVisibleSize();

        _ui = make_autorelease<ui::Menu>();
        this->addChild(_ui);

        _onStartClickConnection = _ui->GetWelcomeView()->OnStartClickConnector(std::bind(&MenuScene::StartClickHandler, this));

        this->scheduleUpdate();

        return true;
    }


    void MenuScene::update(float delta)
    {
        
    }


    void MenuScene::StartClickHandler()
    {
        _ui->SwitchTo(ui::Menu::View::Login);

        _onLoginClickConnection = _ui->GetLoginView()->OnLoginClickConnector(std::bind(&MenuScene::LoginClickHandler, this));
        _onRegisterClickConnection = _ui->GetLoginView()->OnRegisterClickConnector(std::bind(&MenuScene::RegisterClickHandler, this));
    }


    void MenuScene::LoginClickHandler()
    {
        _ui->SwitchTo(ui::Menu::View::Main);

        _onSearchClickConnection = _ui->GetMenuView()->OnSearchClickConnector(std::bind(&MenuScene::SearchClickHandler, this));
    }


    void MenuScene::SearchClickHandler()
    { Director::getInstance()->pushScene(make_autorelease<GameSearchScene>()); }

}
