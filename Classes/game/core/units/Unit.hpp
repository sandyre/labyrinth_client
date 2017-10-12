#ifndef LABYRINTH_CORE_UNITS_UNIT_HPP
#define LABYRINTH_CORE_UNITS_UNIT_HPP

#include "../GameObject.hpp"

namespace labyrinth {
namespace core
{

	class UnitAttributes
	{
	public:
		enum class Attribute : uint32_t
		{
			Input    = 0x01,
			Attack   = 0x02,
			Duelable = 0x04
		};

	public:
		UnitAttributes()
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


	struct DamageDescriptor
	{
		enum class DamageType
		{
			Physical,
			Magical
		};

		Type		Type;
		uint16_t	Value;
		std::string	DealerName;
	};


	class Unit
		: public GameObject
	{
	public:
		using Inventory = std::vector<ItemPtr>;

		enum class Type
		{
			Undefined,
			Monster,
			Player
		};

		enum class Orientation
		{
			Up,
			Down,
			Left,
			Right
		};

		enum class MoveDirection
		{
			Up,
			Down,
			Left,
			Right
		};

		enum class State
		{
			Undefined,
			Walking,
			Swamp,
			Duel,
			Dead
		};

	public:
		Type GetUnitType() const { return _unitType; }
		State GetState() const { return _state; }
		Orientation GetOrientation() const { return _orientation; }
		UnitAttributes GetUnitAttributes() const { return _unitAttributes; }

		std::string GetName() const { return _name; }
		void SetName(const std::string& name) { _name = name; }

		SimpleProperty<> GetDamage() const { return _damage; }
		SimpleProperty<> GetHealth() const { return _health; }
		SimpleProperty<> GetArmor() const { return _armor; }
		SimpleProperty<> GetResistance() const { return _resistance; }

		const Inventory& GetInventory() { return _inventory; }

		virtual void TakeDamage(const DamageDescriptor& dmg);
		virtual void Spawn(const Point<>& pos) override;
		virtual void Respawn(const Point<>& pos);
		virtual void Move(MoveDirection);
		virtual void TakeItem(const ItemPtr& item);
		virtual ItemPtr DropItem(int32_t uid);

		virtual void SpellCast(const GameMessage::CLActionSpell*) = 0;

		virtual void StartDuel(const std::shared_ptr<Unit>& enemy);
		virtual void AcceptDuel(const std::shared_ptr<Unit>& enemy);
		virtual void EndDuel();

		virtual void Die(const std::string& killerName);
		virtual void ApplyEffect(const EffectPtr& effect);

	protected:
		Unit(GameWorld& world, uint32_t uid);

		virtual void Update(float delta) override;

	protected:
		Type					_unitType;
		State					_state;
		Orientation				_orientation;
		UnitAttributes			_unitAttributes;

		SimpleProperty<>		_health;
		SimpleProperty<>		_armor;
		SimpleProperty<>		_resistance;
		SimpleProperty<>		_damage;
		SimpleProperty<float>	_moveSpeed;

		CooldownManager			_cdManager;
		EffectsManager			_effectsManager;

		Inventory				_inventory;
		std::weak_ptr<Unit>		_duelTarget;
	};

}}
using UnitPtr = std::shared_ptr<Unit>;
using UnitWeakPtr = std::weak_ptr<Unit>;

#endif
