//
//  priest.hpp
//  labyrinth
//
//  Created by Aleksandr Borzikh on 03.03.17.
//
//

#ifndef priest_hpp
#define priest_hpp

#include "hero.hpp"

#include <string>
#include <vector>

class Priest : public Hero
{
public:
    static Priest * create(const std::string&);
    
    virtual void                RequestSpellCast1() override {}
    virtual void                SpellCast1(const GameEvent::SVActionSpell*) override {}
protected:
    Priest();
    
    virtual void    update(float) override;
    
protected:
    float m_nRegenInterval;
    float m_nRegenTimer;
    int16_t m_nRegenAmount;
};

#endif /* priest_hpp */
