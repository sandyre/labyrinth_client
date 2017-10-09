//
//  ui_GameScene.hpp
//  labyrinth
//
//  Created by Alexandr Borzykh on 09/10/2017.
//

#ifndef ui_GameScene_hpp
#define ui_GameScene_hpp

#include <cocos2d.h>
#include <ui/CocosGUI.h>


namespace labyrinth {
namespace ui
{
    
    class GameScene
        : public cocos2d::ui::Layout
    {
    public:
        virtual bool init() override;

    private:
    };

}}

#endif /* ui_GameScene_hpp */
