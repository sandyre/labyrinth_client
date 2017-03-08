//
//  NormalSprite.hpp
//  labyrinth
//
//  Created by Aleksandr Borzikh on 08.03.17.
//
//

#ifndef NormalSprite_hpp
#define NormalSprite_hpp

#include <cocos2d.h>

class NormalSprite : public cocos2d::Sprite
{
public:
    static NormalSprite *create(const std::string& filename);
protected:
    NormalSprite();
    ~NormalSprite();
    
    virtual void    draw(cocos2d::Renderer*,
                         const cocos2d::Mat4&,
                         uint32_t) override;
    
    cocos2d::Texture2D  *   m_pNormalMap;
};

#endif /* NormalSprite_hpp */
