//
//  fire_elementalist.hpp
//  labyrinth
//
//  Created by Aleksandr Borzikh on 03.03.17.
//
//

#ifndef fire_elementalist_hpp
#define fire_elementalist_hpp

#include "hero.hpp"

#include <string>
#include <vector>

class FireElementalist : public Hero
{
public:
    static FireElementalist* create(const std::string&);
    
    virtual std::vector<char>   EventSpellCast1() override;
protected:
    FireElementalist();
};

#endif /* fire_elementalist_hpp */
