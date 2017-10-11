#ifndef actionexecutor_hpp
#define actionexecutor_hpp

#include <cocos2d.h>


namespace labyrinth
{

	class AnimationsStorage
	{
	public:
		void Push(const std::string& name, cocos2d::Animate* animation)
		{
			if (_storage.find(name) == _storage.end())
			{
				animation->retain();
				_storage.insert(std::make_pair(name, animation));
			}
			else
				assert(false);
		}

		cocos2d::Animate* Get(const std::string& name)
		{ return _storage[name]; }

	private:
		std::map<std::string, cocos2d::Animate*>  _storage;
	};


	class ActionExecutor
	{
	public:
		enum class ActionType
		{
			Movement,
			Animation
		};

	public:
		ActionExecutor()
			: _target(nullptr)
		{ }

		void SetTarget(cocos2d::Node * target)
		{ assert(target); _target = target; }

		void LaunchAction(cocos2d::Action * action, ActionType actType)
		{
			assert(action);

			if (actType == ActionType::MOVEMENT)
			{
				action->setTag(1);
				action->retain();

				_actionQueue.push(action);
			}
			else if (actType == ActionType::ANIMATION)
			{
				if (_target->getNumberOfRunningActionsByTag(10) != 0)
					_target->stopAllActionsByTag(10);

				action->setTag(10);
				_target->runAction(action);
			}
		}

		void Update()
		{
			if (_target && _target->getNumberOfRunningActionsByTag(1) == 0
				&& !_actionQueue.empty())
			{
				auto action = _actionQueue.front();
				// action->release();
				_target->runAction(action);
				_actionQueue.pop();
			}
		}

	protected:
		cocos2d::Node *                 _target;
		std::queue<cocos2d::Action*>    _actionQueue;
	};

}

#endif
