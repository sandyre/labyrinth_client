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

class UIMainMenuScene : public cocos2d::ui::Layout
{
public:
    UIMainMenuScene();
    
    cocos2d::ui::Text * m_pGameInfo;
    
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
