//
//  gamescene.hpp
//  labyrinth
//
//  Created by Aleksandr Borzikh on 12.01.17.
//
//

#ifndef gamescene_hpp
#define gamescene_hpp

#include "cocos2d.h"
#include "gameworld.hpp"
#include "gamehud.hpp"
#include "ui/ui_gamescene.hpp"

#include "gsnet_generated.h"

class GameScene : public cocos2d::Layer
{
public:
    GameScene();
    ~GameScene();
    virtual void onExit(); // destructor
    
    virtual bool init();
    virtual void update(float);
    
        // for builder (pregamescene)
    GameWorld * GetGameWorld();
    
protected:
    void    UpdateView(float);
    void    UpdateHUD(float);
private:
    UIGameScene *               m_pUI;
    
    GameWorld * m_pGWorld;
};

#endif /* gamescene_hpp */
