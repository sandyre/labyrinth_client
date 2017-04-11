//
//  ui_settingsscene.hpp
//  labyrinth
//
//  Created by Aleksandr Borzikh on 11.04.17.
//
//

#ifndef ui_settingsscene_hpp
#define ui_settingsscene_hpp

#include <cocos2d.h>
#include <UI/CocosGUI.h>

class UISettingsScene : public cocos2d::ui::Layout
{
public:
    UISettingsScene();
    
    cocos2d::ui::Text * m_pSettingsLabel;
    
    cocos2d::ui::Layout * m_pSettingsLayout;
    cocos2d::ui::Text * m_pNicknameLabel;
    cocos2d::ui::TextField * m_pNicknameField;
    
        // back to menu
    cocos2d::ui::Button * m_pBackButton;
};

#endif /* ui_settingsscene_hpp */
