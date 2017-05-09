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
    
    virtual void                RequestSpellCast1() override;
    virtual void                SpellCast1() override;

    virtual void                TakeItem(Item*) override;
protected:
    Rogue();
    
    virtual void update(float) override;
    
protected:
        // Spell cast 1: invisibility
    bool    m_bInvisible;
    float   m_nInvisADuration;
    float   m_nInvisDuration;
};

#endif /* rogue_hpp */
