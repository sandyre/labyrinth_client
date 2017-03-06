//
//  duel_mode.hpp
//  labyrinth
//
//  Created by Aleksandr Borzikh on 19.02.17.
//
//

#ifndef duel_mode_hpp
#define duel_mode_hpp

#include <array>
#include <cocos2d.h>
#include "globals.h"

class DuelMode : public cocos2d::Layer
{
public:
    DuelMode();
    
    void    Show(bool);
    void    TakeInput(InputMove);
    void    Reset();
    void    SetEnemyInfo(std::string name,
                         uint32_t hp);
    bool    ComboDone() const;
protected:
    bool m_bComboDone;
    std::array<InputMove, 5> m_aAttackCombo;
    uint8_t                  m_aCurrentSymbol;
    
        // visible
	std::vector<cocos2d::Sprite *> m_aSymbols;
    cocos2d::Sprite * m_pSymbolSprite;
    cocos2d::Label * m_pCount;
    cocos2d::Label * m_pEnemyInfo;
};

#endif /* duel_mode_hpp */
