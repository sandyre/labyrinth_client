//
//  gameobject.hpp
//  labyrinth
//
//  Created by Aleksandr Borzikh on 21.01.17.
//
//

#ifndef gameobject_hpp
#define gameobject_hpp

#include "../toolkit/Point.hpp"

#include <cocos2d.h>

#include <memory>


class GameWorld;

class GameObject
    : public std::enable_shared_from_this<GameObject>
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
    GameObject(GameWorld& world, uint32_t uid)
    : _world(world),
      _objType(Type::ABSTRACT),
      _uid(uid),
      _sprite(nullptr),
      _objAttributes(Attributes::VISIBLE)
    { }

    virtual ~GameObject()
    {
        if(_sprite)
            _sprite->autorelease();
    }

    GameObject::Type GetType() const
    { return _objType; }

    uint32_t GetUID() const
    { return _uid; }

    std::string GetName() const
    { return _name; }

    void SetName(const std::string& name)
    { _name = name; }

    Point<> GetPosition() const
    { return _pos; }

    void SetPosition(const Point<>& pos)
    { _pos = pos; }

    cocos2d::Sprite * GetSprite()
    {
        assert(_sprite);
        return _sprite;
    }

    virtual void Spawn(const Point<>& pos);
    virtual void Destroy();

    virtual void update(float delta)
    { }

protected:
    GameWorld&          _world;
    GameObject::Type    _objType;
    const uint32_t      _uid;
    std::string         _name;
    uint32_t            _objAttributes;
    Point<>             _pos;
    cocos2d::Sprite *   _sprite;
};
using GameObjectPtr = std::shared_ptr<GameObject>;

#endif /* gameobject_hpp */
