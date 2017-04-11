//
//  settingsscene.hpp
//  labyrinth
//
//  Created by Aleksandr Borzikh on 11.04.17.
//
//

#ifndef settingsscene_hpp
#define settingsscene_hpp

#include <cocos2d.h>
#include "ui/ui_settingsscene.hpp"

class SettingsScene : public cocos2d::Layer
{
public:
    static cocos2d::Scene * createScene();
    
    virtual bool init();
    
    CREATE_FUNC(SettingsScene);
    
protected:
    UISettingsScene * m_pUI;
};

#endif /* settingsscene_hpp */
