#ifndef GP_ENGINE_COLLISION_H
#define GP_ENGINE_COLLISION_H

#include <limits>

#include "objects/Object.h"
#include "objects/Box.h"
#include "objects/Sphere.h"
#include "constraints/HardConstraint.h"

namespace gp
{

namespace engine
{

/**
 * @brief Class detecting and describing a collision of two objects
 */
class Collision final
{
public:
	/**
	 * @brief Possible collision types
	 *
	 * @note Objects will be orderd. E.g. there will never be a
	 *  box-sphere collision.
	 */
	enum CollisionType
	{
		SPHERE_SPHERE,
		SPHERE_BOX,
		BOX_BOX,
		/** Special collision type for "external" constraints */
		CONSTRAINT
	};

private:
	/** The type of the collision (sphere-sphere, sphere-box, ...) */
	const CollisionType m_type;

	/** The constraint that caused the collision (or NULL if is a normal collision) */
	const HardConstraint* const m_constraint;

	/** The first colliding object */
	Object* const m_object1;

	/** The second colliding object */
	Object* const m_object2;

	/** Collision point on object 1 */
	Vector3f m_collisionPoint1;

	/** Collision point ob object 2 */
	Vector3f m_collisionPoint2;

	/**
	 * The normal vector of the collision
	 *
	 * @note The collision normal always points from {@link m_object1}
	 *  to {@link m_object2}.
	 */
	Vector3f m_collisionNormal;

	/** The interpenetration depth */
	float_t m_interpenetrationDepth;

public:
	Collision(Object* object1, Object* object2)
		: m_type(getType(object1, object2)),
		m_constraint(0L),
		m_object1(getFirstObject(object1, object2)),
		m_object2(getSecondObject(object1, object2)),
		m_interpenetrationDepth(std::numeric_limits<float_t>::signaling_NaN())
	{ }

	/**
	 * @brief Create a collision object from a hard constraint
	 */
	Collision(const HardConstraint &constraint)
		: m_type(CONSTRAINT),
		m_constraint(&constraint),
		m_object1(&constraint.object1()),
		m_object2(&constraint.object2()),
		m_interpenetrationDepth(std::numeric_limits<float_t>::signaling_NaN())
	{ }

	/**
	 * @brief Detects a collision and fills the data structure
	 *
	 * {@link collisionPoint1()}, {@link collisionPoint2()}, {@link collisionNormal()} and
	 * {@link interpenetrationDepth()} are only valid after the call of this function
	 * and only if this function returned <code>true</code>.
	 *
	 * @return <code>True</code> of the objects collide
	 *
	 * @note For developers: if this functions returns <code>true</code> the collision
	 *  data has to be valid!
	 */
	bool detect();

	/**
	 * @return The first object of the collision
	 */
	Object* object1() const
	{
		return m_object1;
	}

	/**
	 * @return The second object of the collision
	 */
	Object* object2() const
	{
		return m_object2;
	}

	/**
	 * @return The collision point on surface of the first object
	 * @see {@link detect()}
	 */
	const Vector3f& collisionPoint1() const
	{
		return m_collisionPoint1;
	}

	/**
	 * @return The collision point on surface of the second object
	 * @see {@link detect()}
	 */
	const Vector3f& collisionPoint2() const
	{
		return m_collisionPoint2;
	}

	/**
	 * @return The collision normal pointing from the first object to the second
	 * @see {@link detect()}
	 */
	const Vector3f& collisionNormal() const
	{
		return m_collisionNormal;
	}

	/**
	 * @return The length of the interpenetration along the collision normal
	 * @see {@link detect()}
	 */
	float_t interpenetrationDepth() const
	{
		return m_interpenetrationDepth;
	}

	/**
	 * @return <code>True</code> if friction should be applied to this collision
	 */
	bool applyFriction() const
	{
		// Do not apply friction to ropes and other constraints
		return m_type != CONSTRAINT;
	}

private:
	/**
	 * @brief Detect sphere-sphere collisions
	 */
	bool detectSphereSphere();

	/**
	 * @brief Detect sphere-box collisions
	 */
	bool detectSphereBox();

	/**
	 * @brief Detect box-box collisions
	 */
	bool detectBoxBox();

private:
	static CollisionType getType(Object* o1, Object* o2);

	/**
	 * Helper function to ensure that the first object is a sphere (except for box-box)
	 */
	static Object* getFirstObject(Object* o1, Object* o2);

	/**
	 * Helper function to ensure that the second object is not a sphere (except for sphere-sphere)
	 */
	static Object* getSecondObject(Object* o1, Object* o2);
};

}

}

#endif // GP_ENGINE_COLLISION_H