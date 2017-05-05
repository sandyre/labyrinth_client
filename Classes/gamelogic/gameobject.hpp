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
#include "globals.h"

class GameWorld;

class GameObject : public cocos2d::Sprite
{
public:
    struct Attributes
    {
        static const int MOVABLE = 0x01;
        static const int DUELABLE = 0x02;
        static const int VISIBLE = 0x04;
        static const int DAMAGABLE = 0x08;
        static const int PASSABLE = 0x10;
    };
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
    
    void            SetGameWorld(GameWorld*);
    
    uint32_t        GetAttributes() const;
    
    uint32_t        GetUID() const;
    void            SetUID(uint32_t);
    
    cocos2d::Vec2   GetLogicalPosition() const;
    void            SetLogicalPosition(cocos2d::Vec2);
protected:
        // no need for automatic memory management, gameworld will outlive GO anyway
    GameWorld *     m_poGameWorld;
    
    GameObject::Type    m_eObjType;
    uint32_t            m_nAttributes;
    uint32_t            m_nUID;
    cocos2d::Vec2       m_stLogPosition;
};

#endif /* gameobject_hpp */
