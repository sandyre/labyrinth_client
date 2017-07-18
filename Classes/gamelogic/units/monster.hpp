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

class Monster : public Unit
{
public:
    static Monster* create(const std::string&);
    
    virtual void RequestSpellCast(int) override {}
    virtual void SpellCast(const GameMessage::SVActionSpell*);
protected:
    virtual void update(float) override {}
    
    Monster();
};

#endif /* monster_hpp */
