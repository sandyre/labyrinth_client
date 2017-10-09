//
//  gameobject.hpp
//  labyrinth
//
//  Created by Aleksandr Borzikh on 21.01.17.
//
//

#ifndef gameobject_hpp
#define gameobject_hpp

#include "toolkit/Point.hpp"

#include <cocos2d.h>

#include <memory>


// Static storage (only 1 instance of each animation for CORRECT restoring of original frame)
// FIXME: add 'release()' in destructor
class AnimationsStorage
{
public:
    void Push(const std::string& name, cocos2d::Animate* animation)
    {
        if (_storage.find(name) == _storage.end())
        {
            animation->retain();
            _storage.insert(std::make_pair(name, animation));
        }
        else
            assert(false);
    }

    cocos2d::Animate* Get(const std::string& name)
    { return _storage[name]; }

private:
    std::map<std::string, cocos2d::Animate*>  _storage;
};

// FIXME: memory leaks because of commented 'release()
class ActionExecutor
{
public:
    enum class ActionType
    {
        MOVEMENT,
        ANIMATION
    };

public:
    ActionExecutor()
    : _target(nullptr)
    { }

    void SetTarget(cocos2d::Node * target)
    { assert(target); _target = target; }

    void LaunchAction(cocos2d::Action * action, ActionType actType)
    {
        assert(action);

        if (actType == ActionType::MOVEMENT)
        {
            action->setTag(1);
            action->retain();

            _actionQueue.push(action);
        }
        else if (actType == ActionType::ANIMATION)
        {
            if (_target->getNumberOfRunningActionsByTag(10) != 0)
                _target->stopAllActionsByTag(10);

            action->setTag(10);
            _target->runAction(action);
        }
    }

    void Update()
    {
        if (_target && _target->getNumberOfRunningActionsByTag(1) == 0
            && !_actionQueue.empty())
        {
            auto action = _actionQueue.front();
//            action->release();
            _target->runAction(action);
            _actionQueue.pop();
        }
    }

protected:
    cocos2d::Node *                 _target;
    std::queue<cocos2d::Action*>    _actionQueue;
};

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
    { _actionExecutor.Update(); }

protected:

protected:
    GameWorld&          _world;
    GameObject::Type    _objType;
    const uint32_t      _uid;
    std::string         _name;
    uint32_t            _objAttributes;
    Point<>             _pos;

    cocos2d::Sprite *               _sprite;
    ActionExecutor                  _actionExecutor;
    AnimationsStorage               _animationStorage;
};
using GameObjectPtr = std::shared_ptr<GameObject>;

#endif /* gameobject_hpp */
