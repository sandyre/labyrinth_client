//
//  mainmenuscene.hpp
//  labyrinth
//
//  Created by Aleksandr Borzikh on 20.01.17.
//
//

#ifndef mainmenuscene_hpp
#define mainmenuscene_hpp

#include "cocos2d.h"

class MainMenuScene : public cocos2d::Layer
{
public:
    static cocos2d::Scene * createScene();
    
    virtual bool init();
    
    void enterGameCallback(cocos2d::Ref * pSender);
    
    CREATE_FUNC(MainMenuScene);
};

#endif /* mainmenuscene_hpp */
