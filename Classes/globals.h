//
//  globals.h
//  labyrinth
//
//  Created by Aleksandr Borzikh on 14.01.17.
//
//

#ifndef globals_h
#define globals_h

#include <array>
#include <string>

const int CELL_SIZE = 10;
const int PACKET_MAX_SIZE = 128;

enum class InputMove : int
{
    UP = 0x00,
    DOWN = 0x01,
    LEFT = 0x02,
    RIGHT = 0x03,
    _MIN = 0x00,
    _MAX = 0x03,
};
    
const std::array<std::string, 4> SpritesPath
{
    "res/arrow_up.png",
    "res/arrow_down.png",
    "res/arrow_left.png",
    "res/arrow_right.png"
};

inline
cocos2d::Vec2 LOG_TO_PHYS_COORD(const cocos2d::Vec2& logical,
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
