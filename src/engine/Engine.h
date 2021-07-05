#ifndef GP_ENGINE_ENGINE_H
#define GP_ENGINE_ENGINE_H

#include "config.h"
#include "Collision.h"
#include "ObjectManager.h"
#include "common/Clock.h"
#include "common/messages/Message.h"
#include "common/messages/Queue.h"

namespace gp
{

/**
 * @brief The physics engine
 */
namespace engine
{

class ObjectManager;
class ConstraintManager;

/**
 * @brief The main physics engine class
 */
class Engine
{
private:
	/** Queue for incoming messages */
	gp::messages::Queue<gp::messages::Message>& m_inQueue;
	/** Queue for outgoing messages */
	gp::messages::Queue<gp::messages::Message>& m_outQueue;

	/** The objets of the current scenario */
	ObjectManager* m_objectManager;

	/** Additional constraints for the current scenario */
	ConstraintManager* m_constraintManager;

	/** True if the engine should continue runing */
	bool m_running;

	/** True if the engine is paused */
	bool m_paused = false;

	/** True if the engine shall do a single time step */
	bool m_step = false;

	/** True if the player is shooting on this iteration **/
	bool m_shooting = false;
	Vector3f m_shootingPos;
	Vector3f m_shootingDir;
	/** The collision of the current time step */
	std::vector<Collision> m_collisions;

  /** Time stepping clock */
	Clock m_clock;

public:
	Engine(gp::messages::Queue<gp::messages::Message>& inQueue,
		gp::messages::Queue<gp::messages::Message>& outQueue)
		: m_inQueue(inQueue), m_outQueue(outQueue),
		m_objectManager(0L), m_constraintManager(0L),
		m_running(false)
	{ }

	/**
	 * @brief Start the physics engine
	 *
	 * This function will only return when the stop message was received.
	 */
	void start();

private:
	/**
	 * @brief Handle an incoming message
	 */
	void handleMessage(const gp::messages::Message& message);

	/**
	 * @brief Update the position and velocity of all objects
	 */
	void updatePositionVelocity();

	/**
	 * @brief Detect all collisions
	 */
	void detectCollisions();

	/**
	 * @brief Resolve interpenetration by moving objects
	 */
	void resolveInterpenetrations();

	/**
	 * @brief Apply the collision impulse to all collisions
	 */
	void applyCollisionImpulse();
};

}

}

#endif // GP_ENGINE_ENGINE_H
