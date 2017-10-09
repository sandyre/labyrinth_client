//
//  GameScene.hpp
//  labyrinth
//
//  Created by Alexandr Borzykh on 09/10/2017.
//

#ifndef GameScene_hpp
#define GameScene_hpp

#include <game/client/gameworld.hpp>

#include <cocos2d.h>


namespace labyrinth
{
    namespace ui { struct GameScene { }; }

    class GameScene
        : public cocos2d::Layer
    {
    public:
        virtual bool init() override;

    private:
        ui::GameScene *     _ui;
        GameWorld *         _world;
    };

}

#endif /* GameScene_hpp */
