//
//  gamemap.cpp
//  labyrinth
//
//  Created by Aleksandr Borzikh on 17.01.17.
//
//

#include "gamemap.hpp"

#include <random>
#include "globals.h"
#include "cocos2d.h"

GameMap::GameMap() :
m_pFloorLayer(new cocos2d::Layer()),
m_pWallsLayer(new cocos2d::Layer())
{
    
}

GameMap::~GameMap()
{
    for(auto i = 0; i < m_oMap.size(); ++i)
    {
        for(auto j = 0; j < m_oMap[i].size(); ++j)
        {
            delete m_oMap[i][j];
        }
    }
}

cocos2d::Layer *
GameMap::GetFloorLayer()
{
    return m_pFloorLayer;
}

cocos2d::Layer *
GameMap::GetWallsLayer()
{
    return m_pWallsLayer;
}

void
GameMap::GenerateMap(const Configuration& settings)
{
    m_stSettings = settings;
    
    auto m_oRandGen = std::mt19937(m_stSettings.nSeed);
    auto m_oRandDistr = std::uniform_int_distribution<>(0, 1000);
    
    std::vector<std::vector<MapBlockType>> tmp_map(m_stSettings.nMapSize * m_stSettings.nRoomSize + 2,
                                                   std::vector<MapBlockType>(m_stSettings.nMapSize * m_stSettings.nRoomSize + 2,
                                                                             MapBlockType::NOBLOCK));
    m_oMap = std::vector<std::vector<MapBlock*>>(m_stSettings.nMapSize * m_stSettings.nRoomSize + 2,
                                                 std::vector<MapBlock*>(m_stSettings.nMapSize * m_stSettings.nRoomSize + 2, nullptr));
    
    struct Cell {
        Cell(uint16_t _x, uint16_t _y, MapBlockType _type) {
            x = _x;
            y = _y;
            type = _type;
        }
        
        uint16_t x;
        uint16_t y;
        MapBlockType type;
    };
    
    struct Room {
        cocos2d::Vec2 coord;
        std::vector<std::vector<MapBlockType>> cells;
    };
    
    std::vector<std::vector<Room>> rooms(m_stSettings.nMapSize, std::vector<Room>(m_stSettings.nMapSize));
    
    bool red = false;
    int n = m_stSettings.nRoomSize;
    
    for (size_t i = 0; i < m_stSettings.nMapSize; i++)
    {
        if (m_stSettings.nMapSize % 2 != 1) {
            red = !red;
        }
        
        for (size_t j = 0; j < m_stSettings.nMapSize; j++)
        {
            rooms[i][j].cells.resize(n, std::vector<MapBlockType>(n, MapBlockType::WALL));
            
                // PRIM GENERATION
            std::vector<Cell> list;
            uint16_t x, y;
            
            x = m_oRandDistr(m_oRandGen) % n;
            y = m_oRandDistr(m_oRandGen) % n;
            
            rooms[i][j].cells[x][y] = MapBlockType::NOBLOCK;
            if (x > 0)
            {
                rooms[i][j].cells[x - 1][y] = MapBlockType::BORDER;
                list.push_back(Cell(x - 1, y, MapBlockType::BORDER));
            }
            if (x < n - 1)
            {
                rooms[i][j].cells[x + 1][y] = MapBlockType::BORDER;
                list.push_back(Cell(x + 1, y, MapBlockType::BORDER));
            }
            if (y > 0)
            {
                rooms[i][j].cells[x][y - 1] = MapBlockType::BORDER;
                list.push_back(Cell(x, y - 1, MapBlockType::BORDER));
            }
            if (y < n - 1)
            {
                rooms[i][j].cells[x][y + 1] = MapBlockType::BORDER;
                list.push_back(Cell(x, y + 1, MapBlockType::BORDER));
            }
            
            while (list.size())
            {
                int rand = m_oRandDistr(m_oRandGen) % list.size();
                Cell cell = list[rand];
                std::vector<Cell> neighbours;
                int count = 0;
                x = cell.x;
                y = cell.y;
                
                if (x > 0)
                {
                    if (rooms[i][j].cells[x - 1][y] == MapBlockType::NOBLOCK)
                    {
                        count++;
                    }
                    else
                    {
                        neighbours.push_back(Cell(x - 1, y, rooms[i][j].cells[x - 1][y]));
                    }
                }
                if (x < n - 1)
                {
                    if (rooms[i][j].cells[x + 1][y] == MapBlockType::NOBLOCK)
                    {
                        count++;
                    }
                    else
                    {
                        neighbours.push_back(Cell(x + 1, y, rooms[i][j].cells[x + 1][y]));
                    }
                }
                if (y < n - 1)
                {
                    if (rooms[i][j].cells[x][y + 1] == MapBlockType::NOBLOCK)
                    {
                        count++;
                    }
                    else
                    {
                        neighbours.push_back(Cell(x, y + 1, rooms[i][j].cells[x][y + 1]));
                    }
                }
                if (y > 0)
                {
                    if (rooms[i][j].cells[x][y - 1] == MapBlockType::NOBLOCK)
                    {
                        count++;
                    }
                    else
                    {
                        neighbours.push_back(Cell(x, y - 1, rooms[i][j].cells[x][y - 1]));
                    }
                }
                if (count != 1)
                {
                    list.erase(list.begin() + rand);
                    rooms[i][j].cells[x][y] = MapBlockType::WALL;
                    continue;
                }
                rooms[i][j].cells[x][y] = MapBlockType::NOBLOCK;
                list.erase(list.begin() + rand);
                for (auto& c : neighbours) {
                    if (c.type == MapBlockType::WALL) {
                        list.push_back(c);
                    }
                }
            }
            
            for (size_t k = 0; k < n; k++)
            {
                for (size_t p = 0; p < n; p++)
                {
                    if (rooms[i][j].cells[k][p] != MapBlockType::NOBLOCK)
                    {
                        if (m_oRandDistr(m_oRandGen) > 900)
                        {
                            rooms[i][j].cells[k][p] = MapBlockType::NOBLOCK;
                        }
                    }
                }
            }
            
            if (red) {
                for (size_t k = 0; k < n; k++)
                {
                    rooms[i][j].cells[k][0] = MapBlockType::NOBLOCK;
                    rooms[i][j].cells[k][n - 1] = MapBlockType::NOBLOCK;
                    rooms[i][j].cells[0][k] = MapBlockType::NOBLOCK;
                    rooms[i][j].cells[n - 1][k] = MapBlockType::NOBLOCK;
                }
            }
            red = !red;
            
            for (size_t k = 0; k < n; k++)
            {
                for (size_t p = 0; p < n; p++)
                {
                    tmp_map[i* n + k + 1][j * n + p + 1] = rooms[i][j].cells[p][k];
                }
            }
        }
    }
    int size = m_stSettings.nMapSize * m_stSettings.nRoomSize + 2;
    for (size_t i = 0; i < size; i++)
    {
        tmp_map[i][0] = MapBlockType::BORDER;
        tmp_map[i][size - 1] = MapBlockType::BORDER;
        tmp_map[0][i] = MapBlockType::BORDER;
        tmp_map[size - 1][i] = MapBlockType::BORDER;
    }
    
    for(auto i = size-1; i >= 0; --i)
    {
        for(auto j = size-1; j >= 0; --j)
        {
            auto& block = m_oMap[i][j];
            if(tmp_map[i][j] == MapBlockType::NOBLOCK)
            {
                block = NoBlock::create("res/floor.png");
                
                cocos2d::Vec2 log_coords(i,j);
                cocos2d::Vec2 spritePos = LOG_TO_PHYS_COORD(log_coords,
                                                            block->getContentSize());
                
                block->SetLogicalPosition(log_coords);
                block->setPosition(spritePos);
                
                m_pFloorLayer->addChild(block);
            }
            else if(tmp_map[i][j] == MapBlockType::WALL)
            {
                auto rand = cocos2d::RandomHelper::random_int(0, 2);
                if(rand == 0)
                {
                    block = WallBlock::create("res/wall_1.png");
                }
                if(rand == 1)
                {
                    block = WallBlock::create("res/wall_2.png");
                }
                if(rand == 2)
                {
                    block = WallBlock::create("res/wall_3.png");
                }
                
                cocos2d::Vec2 log_coords(i,j);
                cocos2d::Vec2 spritePos = LOG_TO_PHYS_COORD(log_coords,
                                                            block->getContentSize());
                
                block->SetLogicalPosition(log_coords);
                block->setPosition(spritePos);
                
                m_pWallsLayer->addChild(block);
            }
            else if(tmp_map[i][j] == MapBlockType::BORDER)
            {
                block = BorderBlock::create("res/walls.png");
                
                cocos2d::Vec2 log_coords(i,j);
                cocos2d::Vec2 spritePos = LOG_TO_PHYS_COORD(log_coords,
                                                            block->getContentSize());
                
                block->SetLogicalPosition(log_coords);
                block->setPosition(spritePos);
                
                m_pWallsLayer->addChild(block);
            }
            else
            {
                assert(0);
            }
        }
    }
}

cocos2d::Vec2
GameMap::GetRandomPosition() const
{
    cocos2d::Vec2 position;
    do
    {
        position.x = cocos2d::RandomHelper::random_int(size_t(0), m_oMap.size()-1);
        position.y = cocos2d::RandomHelper::random_int(size_t(0), m_oMap[position.x].size()-1);
    } while(m_oMap[position.x][position.y]->GetType() != MapBlock::Type::NOBLOCK);
    
    return position;
}
