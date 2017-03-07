//
//  ingamehud.hpp
//  labyrinth
//
//  Created by Aleksandr Borzikh on 08.02.17.
//
//

#ifndef ingamehud_hpp
#define ingamehud_hpp

#include <cocos2d.h>
#include <ui/CocosGUI.h>

class GameHUD : public cocos2d::Layer
{
public:
    GameHUD();
    ~GameHUD();
    
    cocos2d::Sprite * m_pTakeItem;
    cocos2d::Label * m_pHP;
    cocos2d::Label * m_pDMG;
    cocos2d::Label * m_pInventory;
};

#endif /* ingamehud_hpp */
