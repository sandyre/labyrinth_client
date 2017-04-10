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

class UIGameScene : public cocos2d::ui::Layout
{
public:
    UIGameScene();
    
        // stats (left top)
    cocos2d::ui::Layout * m_pStatsLayout;
    cocos2d::ui::Text * m_pHP;
    cocos2d::ui::Text * m_pDamage;
    
        // right bottom
    cocos2d::ui::ImageView * m_pTakeItem;
    cocos2d::ui::Text * m_pStats;
    
        // bottom
};

#endif /* ui_gamescene_hpp */
