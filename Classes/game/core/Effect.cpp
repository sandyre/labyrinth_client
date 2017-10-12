#include "Effect.hpp"


namespace labyrinth {
namespace core
{

	Effect::Effect()
		: _timer(),
		  _state(State::Active),
		  _name("Undefined effect")
	{ }


	void Effect::Update(float delta)
	{
		if (_state == Effect::State::Active)
		{
			_timer -= delta;
			if (_timer <= 0.0f)
				_state = Effect::State::Ended;
		}
	}


	WarriorDash::WarriorDash(float duration, float bonusSpeed)
		: _bonusSpeed(bonusSpeed)
	{
		_timer = duration;
		_name = "WarriorDash";
	}


	void WarriorDash::Start()
	{
		_targetUnit.lock()->_moveSpeed -= _bonusSpeed;
	}


	void WarriorDash::Stop()
	{
		_targetUnit.lock()->_moveSpeed += _bonusSpeed;
	}


	WarriorArmorUp::WarriorArmorUp(float duration, int16_t bonusArmor)
		: _bonusArmor(bonusArmor)
	{
		_timer = duration;
		_name = "WarriorArmorUp";
	}


	void WarriorArmorUp::Start()
	{
		_targetUnit.lock()->_armor += _bonusArmor;
	}


	void WarriorArmorUp::Stop()
	{
		_targetUnit.lock()->_armor -= _bonusArmor;
	}


	MageFreeze::MageFreeze(float duration)
	{
		_timer = duration;
		_name = "MageFreeze";
	}


	void MageFreeze::Start()
	{
		_targetUnit.lock()->_unitAttributes.Set<Attributes::InputEnabled>(false);
	}


	void MageFreeze::Stop()
	{
		_targetUnit.lock()->_unitAttributes.Set<Attributes::InputEnabled>(true);
	}


	DuelInvulnerability::DuelInvulnerability(float duration)
	{
		_timer = duration;
		_name = "DuelInvulnerability";
	}


	void DuelInvulnerability::Start()
	{
		_targetUnit.lock()->_unitAttributes.Set<Attributes::Duelable>(false);
	}


	void DuelInvulnerability::Stop()
	{
		_targetUnit.lock()->_unitAttributes.Set<Attributes::Duelable>(true);
	}


	RespawnInvulnerability::RespawnInvulnerability(float duration)
	{
		_timer = duration;
		_name = "RespawnInvulnerability";
	}


	void RespawnInvulnerability::Start()
	{
		_targetUnit.lock()->_unitAttributes.Set<Attributes::Duelable>(false);
		_targetUnit.lock()->_objAttributes.Set<Attributes::Passable>(true);
	}


	void RespawnInvulnerability::Start()
	{
		_targetUnit.lock()->_unitAttributes.Set<Attributes::Duelable>(true);
		_targetUnit.lock()->_objAttributes.Set<Attributes::Passable>(false);
	}

}}
