//
//  monster.hpp
//  labyrinth
//
//  Created by Aleksandr Borzikh on 17.01.17.
//
//

#ifndef monster_hpp
#define monster_hpp

#include "gameobject.hpp"
#include "globals.h"

class Monster : public GameObject
{
public:
    Monster();
    
    uint16_t    GetUID() const;
    void        SetUID(uint16_t);
    
    virtual void AnimationMoveTo(cocos2d::Vec2);
protected:
    uint16_t    m_nUID;
};

#endif /* monster_hpp */
