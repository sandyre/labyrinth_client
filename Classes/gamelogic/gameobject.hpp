//
//  gameobject.hpp
//  labyrinth
//
//  Created by Aleksandr Borzikh on 21.01.17.
//
//

#ifndef gameobject_hpp
#define gameobject_hpp

#include <cocos2d.h>
#include "EffectSprite.h"
#include "globals.h"

class GameObject : public EffectSprite
{
public:
    enum Type
    {
        UNDEFINED,
        MAPBLOCK,
        ITEM,
        CONSTRUCTION,
        UNIT
    };
    GameObject();
    ~GameObject();
    
    GameObject::Type    GetObjType() const;
    
    uint32_t        GetUID() const;
    void            SetUID(uint32_t);
    
    cocos2d::Vec2   GetLogicalPosition() const;
    void            SetLogicalPosition(cocos2d::Vec2);
protected:
    GameObject::Type    m_eObjType;
    uint32_t            m_nUID;
    cocos2d::Vec2       m_stLogPosition;
};

#endif /* gameobject_hpp */
