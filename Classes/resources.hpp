//
//  resources.hpp
//  labyrinth
//
//  Created by Aleksandr Borzikh on 25.02.17.
//
//

#ifndef resources_hpp
#define resources_hpp

#include <array>

static const std::string TitleFont = "fonts/alagard.ttf";
static const std::string TextFont = "fonts/pw_extended.ttf";

static const std::array<std::string, 4> HeroIcons
{
    "res/units/warrior/icon.png",
    "res/units/mage/icon.png",
    "res/units/rogue/icon.png",
    "res/units/priest/icon.png"
};

static const std::array<std::string, 3> SoundEffects
{
    "res/step.mp3",
    "res/duel_start.mp3",
    "res/duel_win.mp3"
};

static const std::array<std::string, 1> SoundMusic
{
    "res/background.mp3"
};

#endif /* resources_hpp */
