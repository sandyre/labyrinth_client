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
static const std::string TempestaFive = "res/ui/ui_fonts/pf_tempesta_five.ttf";

static const std::array<std::string, 4> HeroPortraits
{
    "res/graphics/unit_warrior.png",
    "res/graphics/unit_mage.png",
    "res/graphics/unit_rogue.png",
    "res/graphics/unit_priest.png"
};

static const std::array<std::string, 4> HeroIcons
{
    "res/ui/ui_icons/ui_icon_warrior.png",
    "res/ui/ui_icons/ui_icon_mage.png",
    "res/graphics/rogue_icon.png",
    "res/graphics/priest_icon.png"
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
