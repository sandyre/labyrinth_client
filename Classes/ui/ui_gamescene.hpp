//
//  ui_gamescene.hpp
//  labyrinth
//
//  Created by Aleksandr Borzikh on 09.04.17.
//
//

#ifndef ui_gamescene_hpp
#define ui_gamescene_hpp

#include <cocos2d.h>
#include <UI/CocosGUI.h>

#include "../gamelogic/units/hero.hpp"

class UIBattleView;
class UISkillsPanel;
class UIBattleLogs;

class UIGameScene : public cocos2d::ui::Layout
{
public:
    UIGameScene();
    
        // stats (left top)
    cocos2d::ui::Layout * m_pStatsLayout;
    cocos2d::ui::ImageView * m_pHPBarBack;
    cocos2d::ui::LoadingBar * m_pHPBar;
    cocos2d::ui::Text * m_pHPText;
    cocos2d::ui::Text * m_pArmor;
    
        // left bottom (selected item)
    cocos2d::ui::ImageView * m_pItemFrame;
    cocos2d::ui::ImageView * m_pSelectedItem;
    
        // right bottom (take item, active button)
    cocos2d::ui::Button * m_pTakeItemButton;
    
        // right center - skills panel
    UISkillsPanel * m_poSkillsPanel;
    
        // right top - battle logs
    UIBattleLogs * m_pBattleLogs;
    
        // Battle view
    UIBattleView * m_poBattleView;
};

class UIBattleLogs : public cocos2d::ui::Layout
{
public:
    UIBattleLogs();
    
    void    AddLogMessage(const std::string&);

protected:
    cocos2d::ui::ListView * m_pListView;
};

class UISkillsPanel : public cocos2d::ui::Layout
{
public:
    UISkillsPanel();
    
    std::vector<cocos2d::ui::Button*> m_aSkillsButtons;
};

class UIActionsView;

class UIBattleView : public cocos2d::ui::Layout
{
public:
    UIBattleView();
    
    void    setActive(bool);
    
    UIActionsView * m_poActionsView;
};

class UIAction;

class UIActionsView : public cocos2d::ui::Layout
{
public:
    UIActionsView();
    
    std::vector<UIAction*>   m_apActions;
};

class UIAction : public cocos2d::ui::Layout
{
public:
    UIAction();
    
    void    InitSequence(InputSequence);
    void    RotateLeft();
        // action icon
    cocos2d::ui::Layout *       m_pSequenceLayout;
    cocos2d::ui::ImageView *    m_pTapeImage;
    cocos2d::ui::ImageView *    m_pIcon;
};

#endif /* ui_gamescene_hpp */
