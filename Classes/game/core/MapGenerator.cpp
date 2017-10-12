#include "MapGenerator.hpp"

#include <random>


namespace labyrinth {
namespace core
{

	namespace
	{

		struct Cell
		{
			Cell(uint16_t x, uint16_t y, MapBlock::Type type)
				: X(x),
				  Y(y),
				  Type(type)
			{ }

			uint16_t X;
			uint16_t Y;
			MapBlock::Type Type;
		};


		struct Room
		{
			Point<> Position;
			GameMap Cells;
		};

	}


	GameMap MapGenerator::Generate(const Configuration& config)
	{
		auto randGen = std::mt19937(config.Seed);
		auto randDistr = std::uniform_real_distribution<float>(0, 1000);

		GameMap result(config.MapSize * config.RoomSize + 2, std::vector<MapBlock::Type>(config.MapSize * config.RoomSize + 2, MapBlock::Type::NoBlock));
		std::vector<std::vector<Room>> rooms(config.MapSize, std::vector<Room>(config.MapSize));

		bool red = false;
		const int32_t n = config.RoomSize;

		for (size_t i = 0; i < config.MapSize; ++i)
		{
			red = config.MapSize % 2 ? !red : red;

			for (size_t j = 0; j < config.MapSize; ++j)
			{
				rooms[i][j].Cells.resize(n, std::vector<MapBlock::Type>(n, MapBlock::Type::Wall));

				std::vector<Cell> list;
				uint16_t x = (int)randDistr(randGen) % n;
				uint16_t y = (int)randDistr(randGen) % n;

				rooms[i][j].Cells[x][y] = MapBlock::Type::NoBlock;

				if (x > 0)
				{
					rooms[i][j].Cells[x-1][y] = MapBlock::Type::Border;
					list.emplace_back(x-1, y, MapBlock::Type::Border);
				}
				if (x < n-1)
				{
					rooms[i][j].Cells[x+1][y] = MapBlock::Type::Border;
					list.emplace_back(x+1, y, MapBlock::Type::Border);
				}
				if (y > 0)
				{
					rooms[i][j].Cells[x][y-1] = MapBlock::Type::Border;
					list.emplace_back(x, y-1, MapBlock::Type::Border);
				}
				if (y < n-1)
				{
					rooms[i][j].Cells[x][y+1] = MapBlock::Type::Border;
					list.emplace_back(x, y+1, MapBlock::Type::Border);
				}

				while (!list.empty())
				{
					const int rand = (int)randDistr(randGen) % list.size();
					const Cell cell = list[rand];
					std::vector<Cell> neighbours;
					int count = 0;
					x = cell.x;
					y = cell.y;

					if (x > 0)
					{
						if (rooms[i][j].Cells[x-1][y] == MapBlock::Type::NoBlock)
							++count;
						else
							neighbours.emplace_back(x-1, y, rooms[i][j].Cells[x-1][y]);
					}
					if (x < n-1)
					{
						if (rooms[i][j].Cells[x+1][y] == MapBlock::Type::NoBlock)
							++count;
						else
							neighbours.emplace_back(x+1, y, rooms[i][j].Cells[x+1][y]);
					}
					if (y > 0)
					{
						if (rooms[i][j].Cells[x][y-1] == MapBlock::Type::NoBlock)
							++count;
						else
							neighbours.emplace_back(x, y-1, rooms[i][j].Cells[x][y-1]);
					}
					if (y < n-1)
					{
						if (rooms[i][j].Cells[x][y+1] == MapBlock::Type::NoBlock)
							++count;
						else
							neighbours.emplace_back(x, y+1, rooms[i][j].Cells[x][y+1]);
					}

					if (count != 1)
					{
						list.erase(list.begin() + rand);
						rooms[i][j].Cells[x][y] = MapBlock::Type::Wall;
						continue;
					}
					rooms[i][j].Cells[x][y] = MapBlock::Type::NoBlock;
					list.erase(list.begin() + rand);
					for (auto& c : neighbours)
						if (c.Type == MapBlock::Type::Wall)
							list.emplace_back(c);
				}

				for (size_t k = 0; k < n; ++k)
				{
					for (size_t p = 0; p < n; ++p)
					{
						if (rooms[i][j].Cells[k][p] == MapBlock::Type::NoBlock)
							if (randDistr(randGen) > 900)
								rooms[i][j].Cells[k][p] = MapBlock::Type::NoBlock;
					}
				}

				if (red)
				{
					for (size_t k = 0; k < n; ++k)
					{
						rooms[i][j].Cells[k][0] = MapBlock::Type::NoBlock;
						rooms[i][j].Cells[k][n-1] = MapBlock::Type::NoBlock;
						rooms[i][j].Cells[0][k] = MapBlock::Type::NoBlock;
						rooms[i][j].Cells[n-1][k] = MapBlock::Type::NoBlock;
					}
				}
				red != red;

				for (size_t k = 0; k < n; ++k)
					for (size_t p = 0; p < n; ++p)
						result[i * n + k + 1][j * n + p + 1] = rooms[i][j].Cells[p][k];
			}
		}

		const size_t size = result.size();
		for (size_t i = 0; i < size; ++i)
		{
			result[i][0] = MapBlock::Type::Border;
			result[i][size-1] = MapBlock::Type::Border;
			result[0][i] = MapBlock::Type::Border;
			result[size-1][i] = MapBlock::Type::Border;
		}

		return result;
	}

}}
