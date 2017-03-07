//
//  air_elementalist.hpp
//  labyrinth
//
//  Created by Aleksandr Borzikh on 03.03.17.
//
//

#ifndef air_elementalist_hpp
#define air_elementalist_hpp

#include "hero.hpp"

#include <string>
#include <vector>

class AirElementalist : public Hero
{
public:
    static AirElementalist* create(const std::string&);
    
    virtual std::vector<char>   EventSpellCast1() override;
    virtual void                SpellCast1();
    bool                        isInvisible() const;
    
    virtual void                TakeItem(Item*) override;
protected:
    AirElementalist();
    
    virtual void update(float) override;
    
protected:
        // Spell cast 1: invisibility
    bool    m_bInvisible;
    float   m_nInvisADuration;
    float   m_nInvisDuration;
};

#endif /* air_elementalist_hpp */
