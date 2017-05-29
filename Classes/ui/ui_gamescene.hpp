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

class UIBattleView;
class UISpellsPanel;
class UIBattleLogs;
class Hero;

struct InputSequence;

class UIGameScene : public cocos2d::ui::Layout
{
public:
    UIGameScene();
    
    void    ConfigureForHero(Hero *);
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
    UISpellsPanel * m_poSkillsPanel;
    
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

class UISpellsPanel : public cocos2d::ui::Layout
{
public:
    UISpellsPanel();
    
    void    CreateSpell(const std::string&);
    
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
    
    UIAction *  CreateAction();
    
    std::vector<UIAction*>   m_apActions;
};

class UIAction : public cocos2d::ui::Layout
{
public:
    UIAction();
    
    void    SetHighlighted(bool);
    void    SetIcon(const std::string&);
    
        // action icon
    cocos2d::ui::Layout *       m_pSequenceLayout;
    std::vector<cocos2d::ui::ImageView*>    m_pSequenceSymbols;
    cocos2d::ui::ImageView *    m_pTapeImage;
    cocos2d::ui::ImageView *    m_pIcon;
};

#endif /* ui_gamescene_hpp */
