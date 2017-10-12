#ifndef LABYRINTH_CORE_MAPGENERATOR_HPP
#define LABYRINTH_CORE_MAPGENERATOR_HPP

#include "MapBlock.hpp"

#include <cstdint>


namespace labyrinth {
namespace core
{

	using GameMap = std::vector<std::vector<MapBlock::Type>>;

	class MapGenerator
	{
	public:
		struct Configuration
		{
			uint16_t MapSize;
			uint16_t RoomSize;
			uint32_t Seed;
		};

	public:
		static GameMap Generate(const Configuration& config);
	};

}}

#endif
