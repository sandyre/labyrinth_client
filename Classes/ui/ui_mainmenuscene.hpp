//
//  ui_mainmenuscene.hpp
//  labyrinth
//
//  Created by Aleksandr Borzikh on 06.04.17.
//
//

#ifndef ui_mainmenuscene_hpp
#define ui_mainmenuscene_hpp

#include "../globals.h"
#include <cocos2d.h>
#include <UI/CocosGUI.h>

class UINewsLayout;
class UIStartPage;
class UILoginPage;
class UIMainMenuPage;

class UIMainMenuScene : public cocos2d::ui::Widget
{
public:
    UIMainMenuScene();
    
    cocos2d::ui::PageView * m_pPageView;
    UIStartPage * m_pStartPage;
    UILoginPage * m_pLoginPage;
    UIMainMenuPage * m_pMainPage;
};

class UIStartPage : public cocos2d::ui::Layout
{
public:
    UIStartPage();
    
    cocos2d::ui::Text * m_pGameTitle;
    cocos2d::ui::Button * m_pStartButton;
};

class UILoginPage : public cocos2d::ui::Layout
{
public:
    UILoginPage();
    
    cocos2d::ui::Text * m_pLoginPageLabel;
    
    cocos2d::ui::Button * m_pRegButton;
    cocos2d::ui::Button * m_pLogInButton;
    
    cocos2d::ui::Layout * m_pFormLayout;
    cocos2d::ui::Text * m_pMailLabel;
    cocos2d::ui::TextField * m_pMailField;
    
    cocos2d::ui::Text * m_pPasswordLabel;
    cocos2d::ui::TextField * m_pPasswordField;
};

class UIMainMenuPage : public cocos2d::ui::Layout
{
public:
    UIMainMenuPage();
    
    UINewsLayout * m_pNewsLayout;
    
    cocos2d::ui::Layout * m_pButtonsLayout; // contains central buttons
    cocos2d::ui::Button * m_pPlayButton;
    cocos2d::ui::Button * m_pCollectionButton;
    
    cocos2d::ui::Button * m_pShopButton;
    cocos2d::ui::Button * m_pSettingsButton;
};

class UINewsLayout : public cocos2d::ui::Layout
{
public:
    UINewsLayout();
    
    cocos2d::ui::Text * m_pNewsLabel;
    cocos2d::ui::Button * m_pReloadButton;
    
    cocos2d::ui::ListView * m_pNewsListView;
    std::vector<cocos2d::ui::Text*> m_pNewsArray;
};

#endif /* ui_mainmenuscene_hpp */
