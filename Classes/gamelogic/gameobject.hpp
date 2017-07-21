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


class GameWorld;

class GameObject
{
public:
    struct Attributes
    {
        static const int MOVABLE = 0x01;
        static const int VISIBLE = 0x02;
        static const int DAMAGABLE = 0x04;
        static const int PASSABLE = 0x08;
    };

    enum class Type
    {
        ABSTRACT,
        MAPBLOCK,
        ITEM,
        CONSTRUCTION,
        UNIT
    };

public:
    GameObject(GameWorld * world, uint32_t uid)
    : _world(world),
      _objType(Type::ABSTRACT),
      _uid(uid),
      _sprite(nullptr),
      _objAttributes(Attributes::VISIBLE)
    { }

    virtual ~GameObject()
    { }

    GameObject::Type GetType() const
    { return _objType; }

    uint32_t GetUID() const
    { return _uid; }

    cocos2d::Vec2 GetPosition() const
    { return _pos; }

    void SetPosition(const cocos2d::Vec2& pos)
    { _pos = pos; }

    cocos2d::Sprite * GetSprite()
    {
        assert(_sprite);
        return _sprite;
    }

    virtual void update(float delta) = 0;

    /*
     * Factory method
     */
    template<typename T>
    static T * create(GameWorld * world, uint32_t uid, Args...&& args)
    { return new(std::nothrow) T(world, uid, std::forward<Args>(args)...); }

protected:
    GameWorld *         _world;
    GameObject::Type    _objType;
    const uint32_t      _uid;
    uint32_t            _objAttributes;
    cocos2d::Vec2       _pos;
    cocos2d::Sprite *   _sprite;
};

#endif /* gameobject_hpp */
