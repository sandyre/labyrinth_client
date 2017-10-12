#ifndef LABYRINTH_CORE_EFFECT_HPP
#define LABYRINTH_CORE_EFFECT_HPP

#include <chrono>
#include <cstdint>
#include <memory>

namespace labyrinth {
namespace core
{

	class Effect
	{
	public:
		enum class State
		{
			Active = 0x00,
			Ended  = 0x01,
			Paused = 0x02
		};

	public:
		Effect();
		virtual ~Effect() { }

		State GetState() const { return _state; }
		std::string GetName() const { return _name; }

		void SetTargetUnit(std::weak_ptr<Unit> target) { _target = target; }

		virtual void Start() = 0;
		virtual void Update(float delta);
		virtual void Stop() = 0;

	protected:
		State			_state;
		float			_timer;
		UnitWeakPtr		_targetUnit;
		std::string		_name;
	};


	class WarriorDash
		: public Effect
	{
	public:
		WarriorDash(float duration, float bonusSpeed);

		virtual void Start() override;
		virtual void Stop() override;

	private:
		float		_bonusSpeed;
	};


	class WarriorArmorUp
		: public Effect
	{
	public:
		WarriorArmorUp(float duration, int16_t bonusArmor);

		virtual void Start() override;
		virtual void Stop() override;

	private:
		int16_t		_bonusArmor;
	};


	class MageFreeze
		: public Effect
	{
	public:
		MageFreeze(float duration);

		virtual void Start() override;
		virtual void Stop() override;
	};


	class DuelInvulnerability
		: public Effect
	{
	public:
		DuelInvulnerability(float duration);

		virtual void Start() override;
		virtual void Stop() override;
	};


	class RespawnInvulnerability
		: public Effect
	{
	public:
		RespawnInvulnerability(float duration);

		virtual void Start() override;
		virtual void Stop() override;
	};

}}

#endif
