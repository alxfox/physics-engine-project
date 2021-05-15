#ifndef GP_ENGINE_CONSTRAINTS_HARDCONSTRAINT_H
#define GP_ENGINE_CONSTRAINTS_HARDCONSTRAINT_H

#include "engine/utils.h"
#include "engine/objects/Object.h"

namespace gp
{

namespace engine
{
/**
 * @brief Abstract class for hard constraints between two objects
 *
 * Hard constraints (e.g. ropes) can be applied between to objects.
 * Hard constraints produce collision data like normal collisions.
 */
class HardConstraint
{
protected:
	/** The first object of the constraint */
	Object& m_object1;

	/** The second object of the constraint */
	Object& m_object2;

	/** The point on object 1 where the constraint is attached (in object1 space) */
	const Vector3f m_objPoint1;

	/** The point on object 2 where the constraint is attached (in object2 space) */
	const Vector3f m_objPoint2;

protected:
	/**
	 * @param object1 The first object of the constraint
	 * @param object2 The second object of the constraint
	 * @param objPoint1 The point on object 1 where the constraint is attached
	 * @param objPoint2 The point on object 2 where the constraint is attached
	 */
	HardConstraint(Object &object1, Object &object2,
			const Vector3f &objPoint1 = gp::engine::Vector3f::Zero(),
			const Vector3f &objPoint2 = gp::engine::Vector3f::Zero())
		: m_object1(object1), m_object2(object2),
		m_objPoint1(objPoint1), m_objPoint2(objPoint2)
	{ }

public:
	virtual ~HardConstraint()
	{ }

	/**
	 * Compute the collision data (if a collision is detected)
	 *
	 * @param[out] collisionNormal The collision normal of the collision
	 * @param[out] collisionPoint1 The collision point on object 1
	 * @param[out] collisionPoint2 The collision point on object 2
	 * @param[out] interpenetrationDepth The interpenetration depth
	 * @return <code>True</code> if a collision data was generated
	 */
	virtual bool collision(Vector3f &collisionNormal, Vector3f &collisionPoint1, Vector3f &collisionPoint2,
		float_t &interpenetrationDepth) const = 0;

	Object& object1() const
	{
		return m_object1;
	}

	Object& object2() const
	{
		return m_object2;
	}

	Vector3f computeCollisionPoint1() const {
		// TODO
		return m_object1.position();
	}

	Vector3f computeCollisionPoint2() const {
		// TODO
		return m_object2.position();
	}

};

}

}

#endif // GP_ENGINE_CONSTRAINTS_HARDCONSTRAINT_H
