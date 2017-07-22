//
//  warrior.hpp
//  labyrinth
//
//  Created by Aleksandr Borzikh on 04.03.17.
//
//

#ifndef warrior_hpp
#define warrior_hpp

#include "hero.hpp"

#include <string>
#include <vector>

class Warrior
    : public Hero
{
public:
    Warrior(GameWorld * world, uint32_t uid, const std::string& sprite);
    
    virtual void    update(float delta) override;
    
    virtual void RequestSpellCast(int index) override;
    virtual void SpellCast(const GameMessage::SVActionSpell*) override;
    
    virtual void Move(const GameMessage::SVActionMove*) override;
};

#endif /* warrior_hpp */
