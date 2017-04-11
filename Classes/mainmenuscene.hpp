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
    enum SceneState
    {
        START_PAGE,
        LOGIN_PAGE,
        MAIN_PAGE
    };
public:
    static cocos2d::Scene * createScene();
    
    virtual void update(float delta);
    virtual bool init();
    
    CREATE_FUNC(MainMenuScene);
    
protected:
    SceneState m_eSceneState;
    float      m_fTimer;
    UIMainMenuScene * m_pUI;
};

#endif /* mainmenuscene_hpp */
