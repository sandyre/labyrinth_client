//
//  water_elementalist.hpp
//  labyrinth
//
//  Created by Aleksandr Borzikh on 03.03.17.
//
//

#ifndef water_elementalist_hpp
#define water_elementalist_hpp

#include "hero.hpp"

#include <string>
#include <vector>

class WaterElementalist : public Hero
{
public:
    static WaterElementalist* create(const std::string&);
    
    virtual std::vector<char>   EventSpellCast1() override;
    virtual void                SpellCast1();
protected:
    WaterElementalist();
    
    virtual void    update(float) override;
    
protected:
    bool    m_bDashing;
    float   m_nDashDuration;
    float   m_nDashADuration;
};

#endif /* water_elementalist_hpp */
