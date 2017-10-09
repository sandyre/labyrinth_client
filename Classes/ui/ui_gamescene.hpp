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
#include <ui/CocosGUI.h>

class UIBattleView;
class UISpellsPanel;
class UIBattleLogs;
class Hero;
class UIHeroStats;

struct InputSequence;

class UIGameScene : public cocos2d::ui::Layout
{
public:
    UIGameScene();
    
    void ConfigureForHero(Hero *);
        // stats (left top)
    UIHeroStats * _heroStats;
    
        // right bottom (take item, active button)
    cocos2d::ui::Button * m_pTakeItemButton;
    
        // right center - skills panel
    UISpellsPanel * m_poSkillsPanel;
    
        // right top - battle logs
    UIBattleLogs * m_pBattleLogs;
    
        // Battle view
    UIBattleView * m_poBattleView;
};

class UIHeroStats : public cocos2d::ui::Layout
{
public:
    UIHeroStats();

    void SetHP(int val, int max);
    void SetArmor(int val);
    void SetMagResistance(int val);
    void SetPhysicalDamage(int val);
    void SetMagicalDamage(int val);
    
private:
    cocos2d::ui::ImageView *    _hpBarBack;
    cocos2d::ui::LoadingBar *   _hpBarFront;
    cocos2d::ui::Text *         _hpBarText;

    cocos2d::ui::ImageView *    _armorIcon;
    cocos2d::ui::Text *         _armorText;

    cocos2d::ui::ImageView *    _magResistIcon;
    cocos2d::ui::Text *         _magResistText;

    cocos2d::ui::ImageView *    _physDamageIcon;
    cocos2d::ui::Text *         _physDamageText;

    cocos2d::ui::ImageView *    _magDamageIcon;
    cocos2d::ui::Text *         _magDamageText;
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
    void    ShiftLeft();
    void    Fill(InputSequence);
    void    Clear();
    
        // action icon
    cocos2d::ui::Layout *       m_pSequenceLayout;
    std::vector<cocos2d::ui::ImageView*>    m_pSequenceSymbols;
    cocos2d::ui::ImageView *    _mainFrame;
    cocos2d::ui::ImageView *    _rightFrame;
    cocos2d::ui::ImageView *    _iconBack;
    cocos2d::ui::ImageView *    m_pIcon;
};

#endif /* ui_gamescene_hpp */
