#ifndef LABYRINTH_CORE_ITEM_HPP
#define LABYRINTH_CORE_ITEM_HPP

#include "GameObject.hpp"


namespace labyrinth {
namespace core
{

	class Item
		: public GameObject
	{
	public:
		enum class Type
		{
			Undefined = 0x00,
			Key       = 0x00,
			Sword     = 0x01
		};

		Type GetType() const { return _type; }

	protected:
		Item(GameWorld& world, uint32_t uid)
			: GameObject(world, uid),
			  _type(Type::Undefined)
		{
			_objType = GameObject::Type::Item;
			_objAttributes.Set<Attributes::Passable>(true);
		}

	protected:
		Type		_type;
	};


	class Key
		: public Item
	{
	public:
		Key(GameWorld& world, uint32_t uid)
			: Item(world, uid)
		{ _type = Item::Type::Key; }
	};


	class Sword
		: public Item
	{
	public:
		Sword(GameWorld& world, uint32_t uid)
			: Item(world, uid)
		{ _type = Item::Type::Sword; }
	};

}}

#endif
