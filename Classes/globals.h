//
//  globals.h
//  labyrinth
//
//  Created by Aleksandr Borzikh on 14.01.17.
//
//

#ifndef globals_h
#define globals_h

#include "toolkit/Point.hpp"

#include <cocos2d.h>

#include <array>
#include <string>

const int CELL_SIZE = 10;
const int PACKET_MAX_SIZE = 128;

const int GAMEVERSION_MAJOR = 2;
const int GAMEVERSION_MINOR = 0;
const int GAMEVERSION_BUILD = 0;

enum class InputMove : int
{
    UP = 0x00,
    DOWN = 0x01,
    LEFT = 0x02,
    RIGHT = 0x03,
    _MIN = 0x00,
    _MAX = 0x03,
};
    
enum class InputEvent
{
    SWIPE_UP = 0x00,
    SWIPE_DOWN = 0x01,
    SWIPE_LEFT = 0x02,
    SWIPE_RIGHT = 0x03,
    TAKE_ITEM_BUTTON_CLICK = 0x04,
    SPELL_CAST_0_CLICK = 0x05
};
    
const std::array<std::string, 4> BattleSwipeSprites
{
    "res/ui/ui_combo/ui_combo_arrow_up.png",
    "res/ui/ui_combo/ui_combo_arrow_down.png",
    "res/ui/ui_combo/ui_combo_arrow_left.png",
    "res/ui/ui_combo/ui_combo_arrow_right.png"
};
    
const std::array<std::string, 4> HeroNames
{
    "Warrior",
    "Mage",
    "Rogue",
    "Priest"
};

inline
cocos2d::Vec2 LOG_TO_PHYS_COORD(const Point<>& logical,
                                cocos2d::Size sprite_size)
{
    cocos2d::Vec2 result(logical.x,
                         logical.y);
    result *= sprite_size.width;
    result.x += sprite_size.width/2;
    result.y += sprite_size.height/2;
    return result;
}

#endif /* globals_h */
