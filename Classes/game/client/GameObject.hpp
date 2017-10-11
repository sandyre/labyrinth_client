#ifndef gameobject_hpp
#define gameobject_hpp

#include <toolkit/Point.hpp>

#include <cocos2d.h>

#include <memory>

namespace labyrinth
{

	class GameWorld;

	class ObjectAttributes
	{
	public:
		enum Attribute : uint32_t
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
		bool Check() const
		{ return _state & T; }

		template < Attribute T >
		void Set(bool enabled)
		{ _state = enabled ? (_state | T) : (_state & (~T)); }

	private:
		uint32_t	_state;
	};


	class GameObject
		: public std::enable_shared_from_this<GameObject>
	{
	public:
		struct Attributes
		{
			static const int MOVABLE = 0x01;
			static const int VISIBLE = 0x02;
			static const int DAMAGABLE = 0x04;
			static const int PASSABLE = 0x08;
		};

		enum class Type
		{
			UNDEFINED,
			MAPBLOCK,
			ITEM,
			CONSTRUCTION,
			UNIT
		};

	public:
		GameObject(GameWorld& world, uint32_t uid)
			: _world(world),
			  _objType(Type::UNDEFINED),
			  _uid(uid),
			  _sprite(nullptr)
		{
			_objAttributes.Set<Attribute::Visible>(true);
		}

		virtual ~GameObject()
		{
			if (_sprite)
				_sprite->autorelease();
		}

		uint32_t GetUid() const { return _uid; }
		std::string GetName() const { return _name; }

		Point<> GetPosition() const
		{ return _position; }

		void SetPosition(const Point<>& pos)
		{ _position = pos; }

		cocos2d::Sprite * GetSprite() const { return _sprite; }

		virtual void Spawn(const Point<>& pos) = 0;
		virtual void Destroy() = 0;

		virtual void update(float delta)
		{ _actionExecutor.Update(); }

	protected:
		Type					_objType;
		GameWorld&				_world;
		const uint32_t			_uid;
		ObjectAttributes		_objAttributes;
		Point<>					_position;
		std::string				_name;

		cocos2d::Sprite *		_sprite;
		ActionExecutor			_actionExecutor;
		AnimationStorage		_animationStorage;
	};

}

#endif
