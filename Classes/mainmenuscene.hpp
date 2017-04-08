//
//  mainmenuscene.hpp
//  labyrinth
//
//  Created by Aleksandr Borzikh on 20.01.17.
//
//

#ifndef mainmenuscene_hpp
#define mainmenuscene_hpp

#include <cocos2d.h>
#include <UI/CocosGUI.h>
#include "ui/ui_mainmenuscene.hpp"

class MainMenuScene : public cocos2d::Layer
{
public:
    static cocos2d::Scene * createScene();
    
    virtual bool init();
    
    CREATE_FUNC(MainMenuScene);
    
protected:
    UIMainMenuScene * m_pUI;
};

#endif /* mainmenuscene_hpp */
