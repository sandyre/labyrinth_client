//
//  light.hpp
//  labyrinth
//
//  Created by Aleksandr Borzikh on 08.03.17.
//
//

#ifndef light_hpp
#define light_hpp

#include <cocos2d.h>

class Light
{
public:
    enum Type
    {
        POINT,
        SPOT,
        AMBIENT
    };
public:
    Light() {}
    
protected:
    Light::Type     m_eType;
};

class AmbientLight : public Light
{
public:
    AmbientLight();
    
    cocos2d::Color3B    GetColor() const;
    void                SetColor(cocos2d::Color3B);
protected:
    cocos2d::Color3B    m_cColor;
};

#endif /* light_hpp */
