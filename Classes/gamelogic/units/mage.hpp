//
//  mage.hpp
//  labyrinth
//
//  Created by Aleksandr Borzikh on 03.03.17.
//
//

#ifndef mage_hpp
#define mage_hpp

#include "hero.hpp"

#include <string>
#include <vector>


class Mage
    : public Hero
{
public:
    Mage(GameWorld& world, uint32_t uid, const std::string& sprite);

    virtual void RequestSpellCast(int index) override;
    virtual void SpellCast(const GameMessage::SVActionSpell*) override;
};

#endif /* mage_hpp */
