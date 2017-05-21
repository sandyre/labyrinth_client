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

class Warrior : public Hero
{
public:
    static Warrior *   create(const std::string&);
    
    virtual void                RequestSpellCast(int index) override;
    virtual void                SpellCast(const GameEvent::SVActionSpell*) override;
protected:
    Warrior();
    
    virtual void    update(float) override;
};

#endif /* warrior_hpp */
