//
//  duel_mode.cpp
//  labyrinth
//
//  Created by Aleksandr Borzikh on 19.02.17.
//
//

#include "duel_mode.hpp"

DuelMode::DuelMode() :
m_bComboDone(false),
m_aCurrentSymbol(0)
{
    for(auto& symbol : m_aAttackCombo)
    {
        symbol = (InputMove)cocos2d::RandomHelper::random_int((int)InputMove::_MIN,
                                                              (int)InputMove::_MAX);
    }
    
    auto size = cocos2d::Director::getInstance()->getVisibleSize();
    m_pSymbolSprite = cocos2d::Sprite::create();
    m_pSymbolSprite->setTexture(SpritesPath[(int)m_aAttackCombo[m_aCurrentSymbol]]);
    m_pSymbolSprite->setPosition(size.width/2,
                                 size.height/2);
    this->addChild(m_pSymbolSprite);
    
    m_pCount = cocos2d::Label::createWithTTF("0/0",
                                             "fonts/arial.ttf",
                                             20);
    m_pCount->setPosition(size.width * 0.5,
                          size.height * 0.4);
    this->addChild(m_pCount);
    
    m_pEnemyInfo = cocos2d::Label::createWithTTF("Enemy: ",
                                                 "fonts/arial.ttf",
                                                 20);
    m_pEnemyInfo->setPosition(size.width * 0.7,
                              size.height * 0.8);
    this->addChild(m_pEnemyInfo);
}

void
DuelMode::TakeInput(InputMove move)
{
    if(m_bComboDone)
        return;
    
    if(move == m_aAttackCombo[m_aCurrentSymbol])
    {
        m_aCurrentSymbol++;
        
        m_pCount->setString(cocos2d::StringUtils::format("%d/%zu",
                                                         m_aCurrentSymbol,
                                                         m_aAttackCombo.size()));
        
        if(m_aCurrentSymbol == m_aAttackCombo.size())
        {
            m_bComboDone = true;
        }
        else
        {
                // show next sprite
            m_pSymbolSprite->setTexture(SpritesPath[(int)m_aAttackCombo[m_aCurrentSymbol]]);
        }
    }
    else
    {
        Reset();
    }
}

void
DuelMode::SetEnemyInfo(std::string name, uint32_t hp)
{
    m_pEnemyInfo->setString(cocos2d::StringUtils::format("Enemy: %s\nHP: %d",
                                                         name.c_str(), hp));
}

void
DuelMode::Reset()
{
    m_aCurrentSymbol = 0;
    m_bComboDone = false;
    m_pSymbolSprite->setTexture(SpritesPath[(int)m_aAttackCombo[m_aCurrentSymbol]]);
    
        // FIXME: hardcoded!
    m_pCount->setString("0/8");
}

bool
DuelMode::ComboDone() const
{
    return m_bComboDone;
}

void
DuelMode::Show(bool val)
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
