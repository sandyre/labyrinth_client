//
//  gamemap.cpp
//  labyrinth
//
//  Created by Aleksandr Borzikh on 17.01.17.
//
//

#include "gamemap.hpp"

#include "gameworld.hpp"
#include "mapblock.hpp"

#include <cocos2d.h>

#include <random>


void
GameMap::GenerateMap(const Configuration& settings, GameWorld * world)
{
	auto m_oRandGen = std::mt19937(settings.nSeed);
	auto m_oRandDistr = std::uniform_real_distribution<float>(0, 1000);

	std::vector<std::vector<MapBlockType>> tmp_map(settings.nMapSize * settings.nRoomSize + 2,
		std::vector<MapBlockType>(settings.nMapSize * settings.nRoomSize + 2,
			MapBlockType::NOBLOCK));

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

	std::vector<std::vector<Room>> rooms(settings.nMapSize, std::vector<Room>(settings.nMapSize));

	bool red = false;
	int n = settings.nRoomSize;

	for (size_t i = 0; i < settings.nMapSize; i++)
	{
		if (settings.nMapSize % 2 != 1) {
			red = !red;
		}

		for (size_t j = 0; j < settings.nMapSize; j++)
		{
			rooms[i][j].cells.resize(n, std::vector<MapBlockType>(n, MapBlockType::WALL));

			// PRIM GENERATION
			std::vector<Cell> list;
			uint16_t x, y;

			x = (int)m_oRandDistr(m_oRandGen) % n;
			y = (int)m_oRandDistr(m_oRandGen) % n;

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
				int rand = (int)m_oRandDistr(m_oRandGen) % list.size();
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
	int size = settings.nMapSize * settings.nRoomSize + 2;
	for (size_t i = 0; i < size; i++)
	{
		tmp_map[i][0] = MapBlockType::BORDER;
		tmp_map[i][size - 1] = MapBlockType::BORDER;
		tmp_map[0][i] = MapBlockType::BORDER;
		tmp_map[size - 1][i] = MapBlockType::BORDER;
	}

	// create floor
	uint32_t current_block_uid = 0;
	for (auto i = size - 1; i >= 0; --i)
	{
		for (auto j = size - 1; j >= 0; --j)
		{
            auto block = GameObject::create<NoBlock>(world, current_block_uid, "floor.png");

			cocos2d::Vec2 log_coords(i, j);
            block->Spawn(Point<>(i, j));

            world->_objectsStorage.PushObject(block);
			world->_objectsLayer->addChild(block->GetSprite(), 0);

			++current_block_uid;
		}
	}

	for (auto i = size - 1; i >= 0; --i)
	{
		for (auto j = size - 1; j >= 0; --j)
		{
			int wall_numb = 0;
			if (i != 0)
				wall_numb += (tmp_map[i - 1][j] == MapBlockType::NOBLOCK ? 8 : 0);
			else
				wall_numb += 8;
			if (i != size - 1)
				wall_numb += (tmp_map[i + 1][j] == MapBlockType::NOBLOCK ? 2 : 0);
			else
				wall_numb += 2;
			if (j != 0)
				wall_numb += (tmp_map[i][j - 1] == MapBlockType::NOBLOCK ? 4 : 0);
			else
				wall_numb += 4;
			if (j != size - 1)
				wall_numb += (tmp_map[i][j + 1] == MapBlockType::NOBLOCK ? 1 : 0);
			else
				wall_numb += 1;
			if (tmp_map[i][j] == MapBlockType::WALL)
			{
                    // add shadow
                auto shadow = cocos2d::Sprite::createWithSpriteFrameName("wall_shadow.png");
                Point<> shadowlogPos(i, j);
                cocos2d::Vec2 shadow_spritePos = LOG_TO_PHYS_COORD(shadowlogPos,
                                                                   shadow->getContentSize());
                shadow_spritePos.x = shadow_spritePos.x * 2 / 3;
                shadow_spritePos.y = shadow_spritePos.y * 2 / 3;
                shadow->setPosition(shadow_spritePos);
                world->_objectsLayer->addChild(shadow, 1);
                
				int rand = (int)m_oRandDistr(m_oRandGen) < 700 ? 0 : 1;
				std::string file = "wall_" + std::to_string(wall_numb) + "_" + std::to_string(rand) + ".png";

				auto block = GameObject::create<WallBlock>(world, current_block_uid, file);

                block->Spawn(Point<>(i, j));

                world->_objectsStorage.PushObject(block);
				world->_objectsLayer->addChild(block->GetSprite(), 2);
				++current_block_uid;
			}
			else if (tmp_map[i][j] == MapBlockType::BORDER)
			{
				int rand = (int)m_oRandDistr(m_oRandGen) > 700 ? 0 : 1;
				std::string file = "wall_" + std::to_string(wall_numb) + "_" + std::to_string(rand) + ".png";

                auto block = GameObject::create<BorderBlock>(world, current_block_uid, file);

                block->Spawn(Point<>(i, j));

                world->_objectsStorage.PushObject(block);
			    world->_objectsLayer->addChild(block->GetSprite(), 2);
				++current_block_uid;
			}
		}
	}
}
