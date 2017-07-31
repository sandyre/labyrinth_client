//
//  monster.hpp
//  labyrinth
//
//  Created by Aleksandr Borzikh on 17.01.17.
//
//

#ifndef monster_hpp
#define monster_hpp

#include "unit.hpp"


class Monster
    : public Unit
{
public:
    Monster(GameWorld& world, uint32_t uid, const std::string& sprite);

    virtual void update(float) override {}
    
    virtual void RequestSpellCast(int) override {}
    virtual void SpellCast(const GameMessage::SVActionSpell*);
};

#endif /* monster_hpp */
