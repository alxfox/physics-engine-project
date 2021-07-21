#include <cassert>
#include <chrono>
#include <iostream>

#include "Engine.h"
#include "CollisionResolver.h"
#include "ObjectManager.h"
#include "ConstraintManager.h"
#include "config.h"
#include "messages/ControlMessage.h"
#include "messages/ScenarioMessage.h"
#include "common/messages/StopMessage.h"
#include "utils.h"

void gp::engine::Engine::start()
{
	m_running = true;
	m_shooting = false;
	// The clock running with 60 Hz
	m_clock.start(UPDATE_INTERVAL);

	std::chrono::steady_clock::time_point last = std::chrono::steady_clock::now();

	while (m_running) {
		// Print update interval
		std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
#if 0
		std::cout << "Engine frequency: " << 1000.0*1000.0/std::chrono::duration_cast<std::chrono::microseconds>(now-last).count() << " Hz" << std::endl;
#endif
		last = now;

		// Check incoming messages
		while (!m_inQueue.empty())
			handleMessage(m_inQueue.pop());

		if (m_objectManager && (!m_paused || m_step)) {
			m_step = false;

			// Update position/velocity
			updatePositionVelocity();

			// Detect collisions
			detectCollisions();

			// Apply collision impulse
			applyCollisionImpulse();

			// resolveInterpentrations by moving objects
			resolveInterpenetrations();

			//resolveTriggers by sending proper messages to UI 
			resolveTriggers();

			for (unsigned int i = 1; i < COLLISION_RESOLVES_PER_STEP; i++) {
				detectCollisions();
				if (m_collisions.empty())
					// No more collision, we can stop now
					break;

				resolveInterpenetrations();
			}
		}

		// Wait for the next tick
		m_clock.waitTick();
	}
}

void gp::engine::Engine::handleMessage(const gp::messages::Message& message)
{
	if (gp::messages::isType<gp::messages::StopMessage>(message)) {
		m_running = false;
		return;
	}

	if (gp::messages::isType<messages::ScenarioMessage>(message)) {
		// Load scenario
		const messages::ScenarioMessage &scenarioMessage = static_cast<const messages::ScenarioMessage&>(message);
		m_objectManager = &scenarioMessage.objectManager();
		assert(m_objectManager != 0L);
		m_constraintManager = &scenarioMessage.constraintManager();
		assert(m_constraintManager != 0L);

		// Reset the clock
		m_clock.start(UPDATE_INTERVAL);

		// Send message that the scenario was loaded
		m_outQueue.push(messages::ScenarioLoadedMessage());
		return;
	}

	if (gp::messages::isType<messages::ControlMessage>(message)) {
		const messages::ControlMessage& controlMessage = static_cast<const messages::ControlMessage&>(message);
		m_paused = controlMessage.paused();
		return;
	}

	if (gp::messages::isType<messages::StepMessage>(message)) {
		m_step = true;
		return;
	}

	if(gp::messages::isType<messages::ShootMessage>(message)){
		//add an object or sth (dummy class?) that can be called during collision detection
		const messages::ShootMessage& shootMessage = static_cast<const messages::ShootMessage&>(message);
		m_shootingDir=shootMessage.direction();
		m_shootingPos=shootMessage.source();
		m_shooting=true;
		return;
	}

	if(gp::messages::isType<messages::PlayerPositionMessage>(message)){
		//add an object or sth (dummy class?) that can be called during collision detection
		const messages::PlayerPositionMessage& playerMessage = static_cast<const messages::PlayerPositionMessage&>(message);
		m_playerPos = playerMessage.playerPosition();
		return;
	}

	std::cerr << "Engine Warning: Received unknown message." << std::endl;
}

void gp::engine::Engine::updatePositionVelocity()
{
	const std::vector<Object*>& objects = m_objectManager->objects();
	const float_t step = UPDATE_INTERVAL*SLOW_MOTION_FACTOR;
	for (std::vector<Object*>::const_iterator it = objects.cbegin();
		it != objects.cend(); ++it) {
		Object* o = *it;
		if(!o->isMovable()) continue;
		o->updatePositionAndRotation(ACCELERATION,step);
		o->updateVelocity(ACCELERATION, step);
	}
}

void gp::engine::Engine::detectCollisions()
{
	// Delete last collisions
	m_collisions.clear();

	// Get normal object collisions
	const std::vector<Object*>& objects = m_objectManager->objects();

	for (std::vector<Object*>::const_iterator it1 = objects.cbegin();
		it1 != objects.cend(); ++it1) {
		for (std::vector<Object*>::const_iterator it2 = objects.cbegin();
			it2 != objects.cend(); ++it2) {
			Object* o1 = *it1;
			Object* o2 = *it2;

			// Object does not collide with itself
			// We can also stop the inner loop here since
			// we do not need to check o1<->o2 AND o2<->o1
			if (o1 == o2)
				break;

			// Ignore collision of to unmoveable objects
			if (!o1->isMovable() && !o2->isMovable())
				continue;

			// Perform quick collision dectection
			float_t radSum = o1->boundingRadius() + o2->boundingRadius();
			if ((o1->position() - o2->position()).squaredNorm() >= radSum * radSum)
				continue;

			// Create potential collision object
			Collision collision(o1, o2);
			if (collision.detect())
				m_collisions.emplace_back(collision);

		}
	}
	if(m_shooting){
		for (std::vector<Object*>::const_iterator it1 = objects.cbegin();
			it1 != objects.cend(); ++it1) {
			Object* o1 = *it1;
			if(o1->isMovable()){
				Collision coll(o1,m_shootingPos,m_shootingDir);
				if(coll.detect()){
					m_collisions.emplace_back(coll);
					}
			}
		}
	}
	m_shooting=false;
	for (std::vector<Object*>::const_iterator it1 = objects.cbegin();
		it1 != objects.cend(); ++it1) {
		Object* o1 = *it1;
		if(o1->objType() == Object::TRIGGER_PLAYER){
			o1->setPosition(engine::Vector3f(m_playerPos.x(), m_playerPos.y(), m_playerPos.z()));
		}
		else{
			engine::Vector3f objToCam = m_playerPos - o1->position();
			if (objToCam.norm() > o1->distToCam()){
				//o1->setVelocity(objToCam*1.0f);
				//o1->setDistToCam(objToCam.norm());
			}
			//o1->setVelocity(o1->velocity() + (-o1->position() + m_playerPos)*0.01f);
		}
	}
	// Get additional constraints
	const std::vector<HardConstraint*> constraints = m_constraintManager->objects();

	for (std::vector<HardConstraint*>::const_iterator it = constraints.cbegin();
		it != constraints.cend(); ++it) {
		Collision collision(**it);
		if (collision.detect())
			m_collisions.emplace_back(collision);
	}
}

void gp::engine::Engine::resolveInterpenetrations()
{
	for (std::vector<Collision>::const_iterator it = m_collisions.begin();
		it != m_collisions.end(); ++it) {
		CollisionResolver resolver(*it);
		resolver.resolveInterpenetration();
	}
}

void gp::engine::Engine::resolveTriggers()
{
	for (std::vector<Collision>::const_iterator it = m_collisions.begin();
		it != m_collisions.end(); ++it) {
		CollisionResolver resolver(*it);
		//resolver.resolveTriggers();

		switch (resolver.resolveTriggers()) {
		case gp::engine::CollisionResolver::ENEMY_DIES:
			m_outQueue.push(messages::EnemyDeathMessage());
			break;
		case gp::engine::CollisionResolver::ENEMY_HITS:
			m_outQueue.push(messages::EnemyDamageMessage());
			break;
		}

	}
}

void gp::engine::Engine::applyCollisionImpulse()
{
	for (std::vector<Collision>::const_iterator it = m_collisions.begin();
		it != m_collisions.end(); ++it) {
		CollisionResolver resolver(*it);
		resolver.applyCollisionImpulse();
	}
}
