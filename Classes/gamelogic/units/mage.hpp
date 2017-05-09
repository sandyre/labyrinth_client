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
    
    virtual void                RequestSpellCast1() override;
    virtual void                SpellCast1() override;
protected:
    Mage();
    
    virtual void    update(float) override;
    
protected:
    bool    m_bDashing;
    float   m_nDashDuration;
    float   m_nDashADuration;
};

#endif /* mage_hpp */
