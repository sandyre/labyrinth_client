//
//  NormalSprite.cpp
//  labyrinth
//
//  Created by Aleksandr Borzikh on 08.03.17.
//
//

#include "NormalSprite.hpp"

USING_NS_CC;

NormalSprite::NormalSprite() :
m_pNormalMap(nullptr)
{
    
}

NormalSprite::~NormalSprite()
{
    
}

NormalSprite *
NormalSprite::create(const std::string& filename)
{
    NormalSprite * pSprite = new NormalSprite();
    if(pSprite->initWithFile(filename))
    {
        std::string sNormalPath = filename;
        sNormalPath.resize(sNormalPath.size()-4);
        sNormalPath += "_n.png";
        
        pSprite->m_pNormalMap = Director::getInstance()->getTextureCache()->addImage(sNormalPath);
        
        if(pSprite->m_pNormalMap != nullptr)
        {
            pSprite->autorelease();
            return pSprite;
        }
        else
        {
            throw std::exception();
        }
    }
    
    CC_SAFE_DELETE(pSprite);
    return nullptr;
}

void
NormalSprite::draw(Renderer *renderer,
                   const Mat4 &transform,
                   uint32_t flags)
{
    Sprite::draw(renderer, transform, flags);
    renderer->render();
}
