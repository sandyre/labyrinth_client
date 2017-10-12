#ifndef LABYRINTH_CORE_MAPBLOCK_HPP
#define LABYRINTH_CORE_MAPBLOCK_HPP

#include "GameObject.hpp"

namespace labyrinth {
namespace core
{

	class MapBlock
		: public GameObject
	{
	public:
		enum class Type
		{
			Undefined,
			NoBlock,
			Wall,
			Border
		};

	public:
		Type GetType() const { return _type; }

	protected:
		MapBlock(GameWorld& world, uint32_t uid)
			: GameObject(world, uid)
		{ _objType = GameObject::Type::MapBlock; }

	protected:
		Type		_type;
	};


	class NoBlock
		: public MapBlock
	{
	public:
		NoBlock(GameWorld& world, uint32_t uid)
			: MapBlock(world, uid)
		{
			_type = MapBlock::Type::NoBlock;
			_objAttributes.Set<Attributes::Passable>(true);
		}
	};


	class WallBlock
		: public MapBlock
	{
	public:
		WallBlock(GameWorld& world, uint32_t uid)
			: MapBlock(world, uid)
		{ _type = MapBlock::Type::Wall; }
	};


	class BorderBlock
		: public MapBlock
	{
	public:
		BorderBlock(GameWorld& world, uint32_t uid)
			: MapBlock(world, uid)
		{ _type = MapBlock::Type::Border; }
	};

}}

#endif
