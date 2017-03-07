//
//  earth_elementalist.hpp
//  labyrinth
//
//  Created by Aleksandr Borzikh on 04.03.17.
//
//

#ifndef earth_elementalist_hpp
#define earth_elementalist_hpp

#include "hero.hpp"

#include <string>
#include <vector>

class EarthElementalist : public Hero
{
public:
    static EarthElementalist*   create(const std::string&);
    
    virtual std::vector<char>   EventSpellCast1() override;
    virtual void                SpellCast1();
protected:
    EarthElementalist();
    
    virtual void    update(float) override;
};

#endif /* earth_elementalist_hpp */
