#ifndef LABYRINTH_CORE_GAMEOBJECT_HPP
#define LABYRINTH_CORE_GAMEOBJECT_HPP

#include <toolkit/Point.hpp>

#include <cstdint>
#include <memory>


namespace labyrinth {
namespace core
{

	class GameWorld;

	class ObjectAttributes
	{
	public:
		enum class Attribute : uint32_t
		{
			Movable   = 0x01,
			Visible   = 0x02,
			Damagable = 0x04,
			Passable  = 0x08
		}

	public:
		ObjectAttributes()
		: _state()
		{ }

		template < Attribute T >
		bool Get() const
		{ return _state & (uint32_t)T; }

		template < Attribute T >
		void Set(bool enabled)
		{ _state = enabled ? (_state | (uint32_t)T) : (_state & (~(uint32_t)T)); }

	private:
		uint32_t	_state;
	};


	class GameObject
		: public std::enable_shared_from_this<GameObject>
	{
	public:
		enum class Type
		{
			Undefined,
			Mapblock,
			Item,
			Construction,
			Unit
		};

	public:
		GameObject(GameWorld& world, uint32_t uid)
			: _world(world),
			  _objType(Type::UNDEFINED),
			  _uid(uid)
		{
			_objAttributes.Set<Attribute::Visible>(true);
		}

		virtual ~GameObject()
		{ }

		uint32_t GetUid() const { return _uid; }
		std::string GetName() const { return _name; }
		ObjectAttributes GetObjAttributes() const { return _objAttributes; }

		Point<> GetPosition() const { return _position; }
		void SetPosition(const Point<>& pos) { _position = pos; }

		virtual void Spawn(const Point<>& pos) = 0;
		virtual void Destroy() = 0;

		virtual void Update(float delta) = 0;

	protected:
		Type					_objType;
		GameWorld&				_world;
		const uint32_t			_uid;
		ObjectAttributes		_objAttributes;
		Point<>					_position;
		std::string				_name;
	};

}}

#endif
