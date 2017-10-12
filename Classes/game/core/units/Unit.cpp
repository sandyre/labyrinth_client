#include "Unit.hpp"


namespace labyrinth {
namespace core
{

	Unit::Unit(GameWorld& world, uint32_t uid)
		: GameObject(world, uid),
		  _unitType(Type::Undefined),
		  _state(State::Undefined),
		  _orientation(Orientation::Down),
		  _damage(10, 0, 100),
		  _health(50, 0, 50),
		  _armor(2, 0, 100),
		  _resistance(2, 0, 100),
		  _moveSpeed(0.5, 0.0, 1.0)
	{
		_objType = GameObject::Type::Unit;
		_objAttributes.Set<Attribute::Damagable>(true);
		_objAttributes.Set<Attribute::Movable>(true);

		_unitAttributes.Set<Attribute::Input>(true);
		_unitAttributes.Set<Attribute::Attack>(true);
		_unitAttributes.Set<Attribute::Duelable>(true);
	}


	void Unit::ApplyEffect(const EffectPtr& effect)
	{
		_effectsManager.AddEffect(effect);
	}


	void Unit::Update(float delta)
	{
		_effectsManager.Update(delta);
		_cdManager.Update(delta);
	}

}}
