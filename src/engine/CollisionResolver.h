#ifndef GP_ENGINE_COLLISIONRESOLVER_H
#define GP_ENGINE_COLLISIONRESOLVER_H

#include "Collision.h"
#include "utils.h"

namespace gp
{

namespace engine
{

/**
 * @brief Resolves previously detected collisions
 */
class CollisionResolver final
{
private:
	/** The collision data */
	const Collision &m_collision;

public:

	enum TriggerType 
	{	DEFAULT,
		ENEMY_DIES,
		ENEMY_HITS
	};	
	/**
	 * @param collision A collision object that has detected a collision
	 */
	CollisionResolver(const Collision &collision)
		: m_collision(collision)
	{ }
	/**

	 * @brief True if ray has deleted an object in this Collision Resolution 
	 */

	bool hasDeletedObject();

	void updateDeletedObject(bool deleted);
	/**

	 * @brief Resolve the possible triggers 
	 */
	TriggerType resolveTriggers();

	/**
	 * @brief Resolve the interpenetration
	 */
	void resolveInterpenetration();

	/**
	 * @brief Apply the collision impulse
	 */
	void applyCollisionImpulse();

private:
	/**
	 * @brief Apply collision impulse without rotation or friction
	 */
	void applyCollisionImpulseWithoutRotationFriction();

	/**
	 * @brief Apply collision impulse without friction
	 */
	void applyCollisionImpulseWithoutFriction();

	/**
	 * @brief Apply realistic collision impulse
	 */
	void applyRealisticCollisionImpulse();
	
	/**
	 * @brief Apply collision Impulse for a shot
	 */
	void applyCollisionImpulseForRay();

private:
	/**
	 * @brief The multiplier specifies how far the object is moved along the collision
	 * normal.
	 *
	 * A value of 2 displaces the object to the position where it would be if it
	 * bounced away with a coefficient of restitution of 1.
	 *
	 * Also a value of 2 would place the object at a valid position if it's above
	 * an object which stays still on the ground without being able to move in order
	 * to solve the existing collision properly.
	 *
	 * If the opposite colliding object is e. g. a floor, a value of 1 reduces
	 * some flickering effects due to gravitation since the object is directly
	 * placed on top of the floor.
	 *
	 * The best reason to set this value to 1 (which simulates a kind of
	 * compressible object) is that this reduces also flickering effects when
	 * putting boxes over each other next to a wall. then the box would separate,
	 * collide with the wall, be bounced back and so on...
	 */
	static constexpr float_t SOLVE_INTERPENETRATION_MULTIPLIER = 1.0;

};

}

}

#endif // GP_ENGINE_COLLISIONRESOLVER_H