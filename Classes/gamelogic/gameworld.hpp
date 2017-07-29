//
//  gameworld.hpp
//  labyrinth
//
//  Created by Aleksandr Borzikh on 15.04.17.
//
//

#ifndef gameworld_hpp
#define gameworld_hpp

#include "gamemap.hpp"
#include "units/units_inc.hpp"
#include "../globals.h"
#include "../gsnet_generated.h"
#include "../netsystem.hpp"
#include "../ui/ui_gamescene.hpp"

#include <cocos2d.h>

#include <list>
#include <vector>
#include <queue>


class GameObject;
struct GameSessionDescriptor;

class GameWorld
{
private:
    class ObjectsStorage
    {
        using Storage = std::list<GameObjectPtr>;
    public:
        ObjectsStorage(GameWorld& world)
        : _world(world),
          _uidSeq()
        { }

        template<typename T>
        std::shared_ptr<T> Create()
        {
            auto object = std::make_shared<T>(_world);
            object->SetUID(_uidSeq++);
            _storage.push_back(object);

            return object;
        }

        template<typename T>
        std::shared_ptr<T> Create(uint32_t uid)
        {
            auto object = std::make_shared<T>(_world);
            object->SetUID(uid);
            _storage.push_back(object);

            return object;
        }

        /*
         * description: Prefer using Create<> to Create-and-add object to the storage
         * use Push ONLY if it was created by Create<>, but suddenly was removed from the storage (Item mechanics)
         */
        void PushObject(const GameObjectPtr& obj)
        { _storage.push_back(obj); }

        void DeleteObject(const GameObjectPtr& obj)
        { _storage.remove(obj); }

        template<typename T>
        std::vector<std::shared_ptr<T>> Subset()
        {
            std::vector<std::shared_ptr<T>> result;
            for(auto& obj : _storage)
                if(auto cast = std::dynamic_pointer_cast<T>(obj))
                    result.push_back(cast);
            return result;
        }

        template<typename T = GameObject>
        std::shared_ptr<T> FindObject(uint32_t uid)
        {
#ifdef _DEBUG
            // Consistency check (uid should not have duplicates)
            bool is_copy = std::any_of(_storage.begin(),
                                       _storage.end(),
                                       [uid](const GameObjectPtr& obj)
                                       {
                                           return uid == obj->GetUID();
                                       });
            assert(is_copy == false);
#endif
            auto iter = std::find_if(_storage.begin(),
                                     _storage.end(),
                                     [uid](const GameObjectPtr& obj)
                                     {
                                         return obj->GetUID() == uid;
                                     });
            auto obj = *iter;
            if(iter != _storage.end())
                return std::dynamic_pointer_cast<T>(*iter);

            return nullptr;
        }

        /*
         * STL-containers like API
         */

        Storage::iterator Begin()
        { return _storage.begin(); }

        Storage::iterator End()
        { return _storage.end(); }

        size_t Size() const
        { return _storage.size(); }

    private:
        GameWorld&                  _world;
        uint32_t                    _uidSeq;
        std::list<GameObjectPtr>    _storage;
    };

public:
    GameWorld(GameSessionDescriptor&);
    
    virtual void update(float);
    
    cocos2d::Layer * GetView()
    { return _view; }

    void SetHUD(UIGameScene * ui);
    
    const std::shared_ptr<Hero> GetLocalPlayer()
    { return _localPlayer; }

    std::queue<std::vector<uint8_t>>& GetOutgoingMessages()
    { return _outEvents; }

    void PushMessage(const std::vector<uint8_t>& message)
    { _inputMessages.push(message); }

protected:
    void ApplyInputMessages();

protected:
    GameMap::Configuration                  _mapConf;
    std::queue<std::vector<uint8_t>>        _inputMessages;
    std::queue<std::vector<uint8_t>>        _outEvents;
    ObjectsStorage                          _objectsStorage;
        
        // just points to an Unit in vector
    std::shared_ptr<Hero>                   _localPlayer;

    cocos2d::Layer *                        _objectsLayer;
    
    cocos2d::Layer *                        _view;
    UIGameScene *                           _ui;
    
    friend GameMap;
    friend Unit;
    friend Hero;
    friend Warrior;
    friend Mage;
    friend Monster;
};

#endif /* gameworld_hpp */
