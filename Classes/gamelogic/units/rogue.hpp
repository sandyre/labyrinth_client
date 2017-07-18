//
//  rogue.hpp
//  labyrinth
//
//  Created by Aleksandr Borzikh on 03.03.17.
//
//

#ifndef rogue_hpp
#define rogue_hpp

#include "hero.hpp"

#include <string>
#include <vector>

class Rogue : public Hero
{
public:
    static Rogue* create(const std::string&);
    
    virtual void RequestSpellCast(int index) override;
    virtual void SpellCast(const GameMessage::SVActionSpell*) override;

    virtual void TakeItem(Item*) override;
protected:
    Rogue();
    
    virtual void update(float) override;
};

#endif /* rogue_hpp */
