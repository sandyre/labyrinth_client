//
//  unit.cpp
//  labyrinth
//
//  Created by Aleksandr Borzikh on 07.03.17.
//
//

#include "unit.hpp"

#include "../effect.hpp"
#include "../gameworld.hpp"
#include "../item.hpp"
#include "../../gameconfig.hpp"

#include <audio/include/AudioEngine.h>
#include <cocos2d.h>


Unit::Unit(GameWorld& world, uint32_t uid)
: GameObject(world, uid),
  _type(Type::ABSTRACT),
  _state(State::WALKING),
  _orientation(Orientation::DOWN),
  _unitAttributes(Attributes::INPUT | Attributes::ATTACK | Attributes::DUELABLE),
  _baseDamage(10),
  _bonusDamage(),
  _health(50),
  _healthLimit(50),
  _armor(2),
  _resistance(2),
  _moveSpeed(2.0f)
{
    _objType = GameObject::Type::UNIT;
    _objAttributes |= GameObject::Attributes::MOVABLE | GameObject::Attributes::DAMAGABLE;
}


void
Unit::ApplyEffect(const std::shared_ptr<Effect>& effect)
{
    effect->start();
    _effectsManager.AddEffect(effect);
}


void
Unit::update(float delta)
{
    GameObject::update(delta);
    _cdManager.Update(delta);
    _effectsManager.Update(delta);    
}


/*
 *
 * Actions request
 *
 */
void
Unit::RequestMove(MoveDirection dir)
{
    flatbuffers::FlatBufferBuilder builder;
    auto uuid = builder.CreateString(GameConfiguration::Instance().GetUUID());
    auto move = GameMessage::CreateCLActionMove(builder,
                                                this->GetUID(),
                                                (char)dir);
    auto event = GameMessage::CreateMessage(builder,
                                            uuid,
                                            GameMessage::Messages_CLActionMove,
                                            move.Union());
    builder.Finish(event);
    
    _world._outEvents.emplace(builder.GetBufferPointer(),
                               builder.GetBufferPointer() + builder.GetSize());
}


void 
Unit::RequestTakeItem(const std::shared_ptr<Item>& item)
{
    flatbuffers::FlatBufferBuilder builder;
    auto uuid = builder.CreateString(GameConfiguration::Instance().GetUUID());
    auto take = GameMessage::CreateCLActionItem(builder,
                                                this->GetUID(),
                                                item->GetUID(),
                                                GameMessage::ActionItemType_TAKE);
    auto event = GameMessage::CreateMessage(builder,
                                            uuid,
                                            GameMessage::Messages_CLActionItem,
                                            take.Union());
    builder.Finish(event);
        
    _world._outEvents.emplace(builder.GetBufferPointer(),
                               builder.GetBufferPointer() + builder.GetSize());
}


void
Unit::RequestStartDuel(const std::shared_ptr<Unit>& enemy)
{
    flatbuffers::FlatBufferBuilder builder;
    auto uuid = builder.CreateString(GameConfiguration::Instance().GetUUID());
    auto take = GameMessage::CreateCLActionDuel(builder,
                                                this->GetUID(),
                                                enemy->GetUID(),
                                                GameMessage::ActionDuelType_STARTED);
    auto event = GameMessage::CreateMessage(builder,
                                            uuid,
                                            GameMessage::Messages_CLActionDuel,
                                            take.Union());
    builder.Finish(event);
    
    _world._outEvents.emplace(builder.GetBufferPointer(),
                               builder.GetBufferPointer() + builder.GetSize());
}

/*
 *
 * Animations
 *
 */

void
Unit::Spawn(const Point<>& log_pos)
{
    _state = Unit::State::WALKING;
    _objAttributes = GameObject::Attributes::DAMAGABLE | GameObject::Attributes::VISIBLE | GameObject::Attributes::MOVABLE;
    _unitAttributes = Unit::Attributes::ATTACK | Unit::Attributes::DUELABLE | Unit::Attributes::INPUT;
    _health = _healthLimit;
    
    _pos = log_pos;
    _sprite->setPosition(LOG_TO_PHYS_COORD(log_pos, _sprite->getContentSize()));
    
        // animation
    auto fadeIn = cocos2d::FadeIn::create(0.5);
    _sprite->runAction(fadeIn);
}


void
Unit::Respawn(const Point<>& log_pos)
{
    this->Spawn(log_pos);
    
    auto spawnEffect = std::make_shared<RespawnInvulnerability>(std::static_pointer_cast<Unit>(shared_from_this()), 5.0);
    ApplyEffect(spawnEffect);
}


std::shared_ptr<Item>
Unit::DropItem(int32_t uid)
{
    auto item = std::find_if(_inventory.begin(),
                             _inventory.end(),
                             [uid](const std::shared_ptr<Item>& item)
                             {
                                 return item->GetUID() == uid;
                             });

    if(item == _inventory.end())
        return nullptr;

    _world._objectsStorage.PushObject(*item);
    (*item)->Spawn(_pos);

    auto item_ptr = *item;
    _inventory.erase(item);
    return item_ptr;
}


void
Unit::TakeDamage(const Unit::DamageDescriptor& dmg)
{
    int16_t damageTaken = dmg.Value;

    if(dmg.Type == DamageDescriptor::DamageType::PHYSICAL)
        damageTaken -= _armor;
    else if(dmg.Type == DamageDescriptor::DamageType::MAGICAL)
        damageTaken -= _resistance;

    _health -= damageTaken;
    
        // animated text
    auto hp_text = cocos2d::Label::create(cocos2d::StringUtils::format("-%d", damageTaken),
                                          "fonts/alagard.ttf",
                                          12);
    hp_text->setTextColor(dmg.Type == DamageDescriptor::DamageType::PHYSICAL ? cocos2d::Color4B::WHITE : cocos2d::Color4B::MAGENTA);
    
    auto center_pos = _sprite->getContentSize() / 2;
    hp_text->setPosition(center_pos);
    _sprite->addChild(hp_text);
    
    auto moveup = cocos2d::MoveBy::create(3,
                                          cocos2d::Vec2(0, center_pos.height));
    auto fadeout = cocos2d::FadeOut::create(0.5f);
    auto seq = cocos2d::Sequence::create(moveup,
                                         fadeout,
                                         cocos2d::RemoveSelf::create(),
                                         nullptr);
    hp_text->runAction(seq);
}


void
Unit::Die()
{
    if(_duelTarget)
    {
        _duelTarget->EndDuel();
        EndDuel();
    }
    
    _state = Unit::State::DEAD;
    _health = 0;
    _objAttributes = GameObject::Attributes::PASSABLE;
    _unitAttributes = 0;

        // drop items
    auto items = _inventory;
    for(auto item : items)
        this->DropItem(item->GetUID());
    
        // remove all effects
    _effectsManager.RemoveAll();
    
        // animation
    auto fadeOut = cocos2d::FadeOut::create(0.5);
    _sprite->runAction(fadeOut);

    _actionExecutor.LaunchAction(_animationStorage.Get("death"), ActionExecutor::ActionType::ANIMATION);
}


void
Unit::Move(const GameMessage::SVActionMove* mov)
{
    Orientation new_orient = (Orientation)mov->mov_dir();
    MoveDirection mov_dir = (MoveDirection)mov->mov_dir();
    Point<> new_pos = _pos;
    if(mov_dir == MoveDirection::UP)
    {
        ++new_pos.y;
    }
    else if(mov_dir == MoveDirection::DOWN)
    {
        --new_pos.y;
    }
    else if(mov_dir == MoveDirection::LEFT)
    {
        --new_pos.x;
    }
    else if(mov_dir == MoveDirection::RIGHT)
    {
        ++new_pos.x;
    }
    
    // change orientation properly
    if(_orientation != new_orient)
    {
        if(new_orient == Orientation::LEFT)
        {
            _sprite->setFlippedX(true);
        }
        else if(new_orient == Orientation::RIGHT)
        {
            _sprite->setFlippedX(false);
        }
    }

    _orientation = new_orient;

    if(new_pos != Point<>(mov->x(), mov->y()))
    {
        new_pos = Point<>(mov->x(), mov->y());
    }

    _pos = new_pos;

        // animation
    auto moveAnim = _animationStorage.Get("move");
    moveAnim->setDuration(1.0 / _moveSpeed);

    auto moveTo = cocos2d::MoveTo::create(1.0 / _moveSpeed,
                                          LOG_TO_PHYS_COORD(new_pos, _sprite->getContentSize()));

    auto spawn = cocos2d::Spawn::create(moveAnim, moveTo, nullptr);

    _actionExecutor.LaunchAction(spawn, ActionExecutor::ActionType::MOVEMENT);

        // sound
    auto distance = _world.GetLocalPlayer()->GetPosition().Distance(this->GetPosition());
    if(distance <= 10.0)
    {
        auto audio = cocos2d::experimental::AudioEngine::play2d("res/audio/step.mp3",
                                                                false,
                                                                1.0f / distance);
    }
}


void
Unit::TakeItem(const std::shared_ptr<Item>& item)
{ _inventory.push_back(item); }


void
Unit::StartDuel(const std::shared_ptr<Unit>& unit)
{
    // change orientation properly
    if(unit->GetPosition().x > this->GetPosition().x)
    {
        _orientation = Orientation::RIGHT;
        _sprite->setFlippedX(false);
    }
    else
    {
        _orientation = Orientation::LEFT;
        _sprite->setFlippedX(true);
    }

    _state = State::DUEL;
    _unitAttributes &= ~(Unit::Attributes::DUELABLE);

    _duelTarget = unit;
}


void
Unit::EndDuel()
{
    _state = Unit::State::WALKING;
    _unitAttributes |= Unit::Attributes::DUELABLE;
    
    _duelTarget.reset();
}
