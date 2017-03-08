//
//  DynamicLayer.hpp
//  labyrinth
//
//  Created by Aleksandr Borzikh on 08.03.17.
//
//

#ifndef DynamicLayer_hpp
#define DynamicLayer_hpp

#include <cocos2d.h>

class DynamicLayer : public cocos2d::Layer
{
public:
    static DynamicLayer * create();
protected:
    DynamicLayer();
    ~DynamicLayer();
    
    virtual void    draw(cocos2d::Renderer*,
                         const cocos2d::Mat4&,
                         uint32_t) override;
};

#endif /* DynamicLayer_hpp */
