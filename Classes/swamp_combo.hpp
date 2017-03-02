//
//  swamp_combo.hpp
//  labyrinth
//
//  Created by Aleksandr Borzikh on 18.02.17.
//
//

#ifndef swamp_combo_hpp
#define swamp_combo_hpp

#include <array>
#include <cocos2d.h>
#include "globals.h"

class SwampCombo : public cocos2d::Layer
{
public:
    SwampCombo();
    
    void    Show(bool);
    void    TakeInput(InputMove);
    void    Reset();
    bool    ComboDone() const;
protected:
    bool    m_bComboDone;
    std::array<InputMove, 5> m_aCombo;
    uint8_t                  m_aCurrentSymbol;
    
        // visible
    cocos2d::Sprite * m_pSymbolSprite;
    cocos2d::Label * m_pCount;
};

#endif /* swamp_combo_hpp */
