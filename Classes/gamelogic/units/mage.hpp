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

class Mage : public Hero
{
public:
    static Mage* create(const std::string&);
    
    virtual void RequestSpellCast(int index) override;
    virtual void SpellCast(const GameMessage::SVActionSpell*) override;
protected:
    Mage();
    
    virtual void update(float) override;
};

#endif /* mage_hpp */
