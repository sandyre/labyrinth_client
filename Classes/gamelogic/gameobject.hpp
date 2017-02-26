//
//  gameobject.hpp
//  labyrinth
//
//  Created by Aleksandr Borzikh on 21.01.17.
//
//

#ifndef gameobject_hpp
#define gameobject_hpp

#include "cocos2d.h"
#include "globals.h"

class GameObject : public cocos2d::Sprite
{
public:
    enum Type
    {
        UNDEFINED,
        MAPBLOCK,
        ITEM,
        CONSTRUCTION,
        PLAYER,
        MONSTER
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

//class GameObject
//{
//public:
//    enum Type
//    {
//        PLAYER,
//        MONSTER,
//        ITEM,
//        CONSTRUCTION,
//        MAPBLOCK
//    };
//public:
//    GameObject();
//    virtual ~GameObject();
//    
//    GameObject::Type    GetObjectType() const;
//    
//    void                        SetPosition(const cocos2d::Vec2&);
//    const cocos2d::Vec2&        GetPosition() const;
//    
//    bool                InitSprite(const std::string&);
//    cocos2d::Sprite *   GetSprite();
//protected:
//    GameObject::Type    m_eObjType;
//    cocos2d::Vec2       m_stPosition;
//    cocos2d::Sprite *   m_pSprite;
//};

#endif /* gameobject_hpp */
