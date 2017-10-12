#ifndef LABYRINTH_CORE_CONSTRUCTION_HPP
#define LABYRINTH_CORE_CONSTRUCTION_HPP

#include <game/core/GameObject.hpp>


namespace labyrinth {
namespace core
{

	class Construction
		: public GameObject
	{
	public:
		enum class Type
		{
			Undefined = 0x00,
			Door      = 0x00,
			Graveyard = 0x01,
			Fountain  = 0x02
		};

	public:
		Type GetType() const { return _type; }

	protected:
		Construction(GameWorld& world, uint32_t uid)
			: GameObject(world, uid),
			  _type(Type::Undefined)
		{ _objType = GameObject::Type::Construction; }

	protected:
		Type		_type;
	};


	class Door
		: public Construction
	{
	public:
		Door(GameWorld& world, uint32_t uid)
			: Construction(world, uid)
		{ _type = Construction::Type::Door; }
	};


	class Graveyard
		: public Construction
	{
	public:
		Graveyard(GameWorld& world, uint32_t uid)
			: Construction(world, uid)
		{ _type = Construction::Type::Graveyard; }
	};


	class Fountain
		: public Construction
	{
	public:
		Fountain(GameWorld& world, uint32_t uid)
			: Construction(world, uid)
		{
			_type = Construction::Type::Fountain;
			_objAttributes.Set<Attributes::Passable>(true);
		}
	};

}}

#endif
