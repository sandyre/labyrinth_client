//
//  globals.h
//  labyrinth
//
//  Created by Aleksandr Borzikh on 14.01.17.
//
//

#ifndef globals_h
#define globals_h

#include <cocos2d.h>
#include <array>
#include <string>

const int CELL_SIZE = 10;
const int PACKET_MAX_SIZE = 128;

const int GAMEVERSION_MAJOR = 1;
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
    "res/ui/battle/btl_arrow_1.png",
    "res/ui/battle/btl_arrow_2.png",
    "res/ui/battle/btl_arrow_3.png",
    "res/ui/battle/btl_arrow_4.png"
};
    
const std::array<std::string, 4> HeroNames
{
    "Warrior",
    "Mage",
    "Rogue",
    "Priest"
};

struct PlayerInfo
{
    uint32_t    nUID;
    std::string sNickname;
    int         nHeroIndex;
        // additional fields
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
