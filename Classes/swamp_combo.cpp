//
//  swamp_combo.cpp
//  labyrinth
//
//  Created by Aleksandr Borzikh on 18.02.17.
//
//

#include "swamp_combo.hpp"

SwampCombo::SwampCombo() :
m_bComboDone(false),
m_aCurrentSymbol(0)
{
    for(auto& symbol : m_aCombo)
    {
        symbol = (InputMove)cocos2d::RandomHelper::random_int((int)InputMove::_MIN,
                                                              (int)InputMove::_MAX);
    }
    
    auto size = cocos2d::Director::getInstance()->getVisibleSize();
    m_pSymbolSprite = cocos2d::Sprite::create();
    m_pSymbolSprite->setTexture(SpritesPath[(int)m_aCombo[m_aCurrentSymbol]]);
    m_pSymbolSprite->setPosition(size.width/2,
                                 size.height/2);
    this->addChild(m_pSymbolSprite);
    
    m_pCount = cocos2d::Label::createWithTTF("0/0",
                                             "fonts/arial.ttf",
                                             20);
    m_pCount->setPosition(size.width * 0.5,
                          size.height * 0.4);
    this->addChild(m_pCount);
}

void
SwampCombo::TakeInput(InputMove move)
{
    if(m_bComboDone)
        return;
    
    if(move == m_aCombo[m_aCurrentSymbol])
    {
        m_aCurrentSymbol++;
        
        m_pCount->setString(cocos2d::StringUtils::format("%d/%zu",
                                                         m_aCurrentSymbol,
                                                         m_aCombo.size()));
        
        if(m_aCurrentSymbol == m_aCombo.size())
        {
            m_bComboDone = true;
        }
        else
        {
                // show next sprite
            m_pSymbolSprite->setTexture(SpritesPath[(int)m_aCombo[m_aCurrentSymbol]]);
        }
    }
    else
    {
        Reset();
    }
}

void
SwampCombo::Reset()
{
    m_aCurrentSymbol = 0;
    m_bComboDone = false;
    m_pSymbolSprite->setTexture(SpritesPath[(int)m_aCombo[m_aCurrentSymbol]]);
    
        // FIXME: hardcoded!
    m_pCount->setString("0/8");
}

bool
SwampCombo::ComboDone() const
{
    return m_bComboDone;
}

void
SwampCombo::Show(bool val)
{
    if(val)
    {
        this->setVisible(true);
    }
    else
    {
        this->setVisible(false);
    }
}
