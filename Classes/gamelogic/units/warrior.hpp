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
    
    virtual void                RequestSpellCast1() override;
    virtual void                SpellCast1() override;
protected:
    Warrior();
    
    virtual void    update(float) override;
    
protected:
    bool m_bDashing;
    float m_nDashingADuration;
    float m_nDashingDuration;
};

#endif /* warrior_hpp */
