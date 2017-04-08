//
//  ui_pregamescene.hpp
//  labyrinth
//
//  Created by Aleksandr Borzikh on 26.03.17.
//
//

#ifndef ui_pregamescene_hpp
#define ui_pregamescene_hpp

#include "../globals.h"
#include "../gamelogic/units/hero.hpp"
#include <cocos2d.h>
#include <UI/CocosGUI.h>

class UIPlayersList;
class UIPlayerInfo;
class UIHeroPick;

class UIPregameScene : public cocos2d::ui::Layout
{
public:
    UIPregameScene();
    
    cocos2d::ui::Text * m_pStatusText;
    UIHeroPick * m_pHeroPick;
    UIPlayersList * m_pPlayersList;
};

class UIHeroPick : public cocos2d::ui::Layout
{
public:
    UIHeroPick();
    
    int                     m_nHeroIndex;
    cocos2d::ui::Text *     m_pSelectedHeroText;
    cocos2d::ui::ImageView * m_pSelectedHeroImage;
    cocos2d::ui::Button *   m_pRightChange;
    cocos2d::ui::Button *   m_pLeftChange;
};

class UIPlayerInfo : public cocos2d::ui::Layout
{
public:
    UIPlayerInfo();
    
    cocos2d::ui::ImageView *    m_pHeroImage;
    cocos2d::ui::Text      *    m_pPlayerName;
    cocos2d::ui::CheckBox  *    m_pReadyStatus;
    
    PlayerInfo  m_stPlayerInfo;
};

class UIPlayersList : public cocos2d::ui::Layout
{
public:
    UIPlayersList();
    
    void    AddPlayer(const PlayerInfo&);
    void    RemovePlayer(uint32_t uid);

    cocos2d::ui::Text *         m_pPlayersLabel;
    cocos2d::ui::ListView *     m_pListView;
    std::vector<UIPlayerInfo*>  m_aPlayers;
};

#endif /* ui_pregamescene_hpp */
