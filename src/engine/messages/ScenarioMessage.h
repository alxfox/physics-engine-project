#ifndef GP_ENGINE_MESSAGES_STOPMESSAGE_H
#define GP_ENGINE_MESSAGES_STOPMESSAGE_H

#include "common/messages/Message.h"

namespace gp
{

namespace engine
{

class ObjectManager;
class ConstraintManager;

/**
 * @brief Physics engine related messages
 */
namespace messages
{

/**
 * @brief Load a new scenario
 *
 * When this message is received, the physics engine load the new scenario
 * specified by the {@link ObjectManager}.
 */
class ScenarioMessage : public gp::messages::Message
{
private:
	/** The object manager of the scenario */
	ObjectManager &m_objectManager;

	/** The constraint manager of the scenario */
	ConstraintManager &m_constraintManager;

public:
	/**
	 * @param objectManager The object manager describing the new scenario
	 */
	ScenarioMessage(ObjectManager &objectManager, ConstraintManager &constraintManager)
		: gp::messages::Message(type_id<ScenarioMessage>()),
		m_objectManager(objectManager), m_constraintManager(constraintManager)
	{ }

	/**
	 * @brief Get the object manager
	 */
	ObjectManager& objectManager() const
	{
		return m_objectManager;
	}

	/**
	 * @brief Get the constraint manager
	 */
	ConstraintManager& constraintManager() const
	{
		return m_constraintManager;
	}
};

/**
 * @brief Loaded a new scenario
 *
 * When this message is received, the physics engine finished loading a scenario.
 */
class ScenarioLoadedMessage : public gp::messages::Message
{
public:
	Vector3f m_test;
	ScenarioLoadedMessage() : Message(type_id<ScenarioLoadedMessage>()), m_test(Vector3f(1.0f,1.0f,1.0f)) {}
};

class ScoreAndLifeUpdate : public gp::messages::Message
{
private:
	float_t m_score;
	float_t m_life;

public:
	ScoreAndLifeUpdate(float_t score, float_t life): Message(type_id<ScoreAndLifeUpdate>()), m_score(score), m_life(life) {}

	float_t get_score() const {
		return m_score;
	}
	float_t get_life() const {
		return m_life;
	}
};

class EnemyDamageMessage: public gp::messages::Message
{
private:

public:
	EnemyDamageMessage(): Message(type_id<EnemyDamageMessage>()) {}

};

class EnemyDeathMessage: public gp::messages::Message
{
private:

public:
	EnemyDeathMessage(): Message(type_id<EnemyDeathMessage>()) {}

};

}

}

}

#endif // GP_ENGINE_MESSAGES_STOPMESSAGE_H
