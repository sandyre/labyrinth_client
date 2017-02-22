//
//  gamemap.hpp
//  labyrinth
//
//  Created by Aleksandr Borzikh on 17.01.17.
//
//

#ifndef gamemap_hpp
#define gamemap_hpp

#include "gameobject.hpp"
#include "globals.h"
#include "mapblock.hpp"
#include <stack>
#include <vector>

class GameMap
{
    enum class MapBlockType : unsigned char
    {
        NOBLOCK = 0x00,
        WALL    = 0x01,
        BORDER  = 0x02
    };
public:
    struct Configuration
    {
        uint16_t nMapSize;
        uint16_t nRoomSize;
        uint32_t nSeed;
    };
    
    GameMap();
    ~GameMap();
    
    void    GenerateMap(const Configuration&);
    
    std::vector<std::vector<MapBlock*>>& GetMatrix()
    {
        return m_oMap;
    }
    
    std::vector<MapBlock*>& operator[](int i)
    {
        return m_oMap[i];
    }
    
    cocos2d::Layer *    GetFloorLayer();
    cocos2d::Layer *    GetWallsLayer();
    
    cocos2d::Vec2       GetRandomPosition() const;
private:
    Configuration                       m_stSettings;
    cocos2d::Layer *                    m_pFloorLayer;
    cocos2d::Layer *                    m_pWallsLayer;
    std::vector<std::vector<MapBlock*>> m_oMap;
};

#endif /* gamemap_hpp */
