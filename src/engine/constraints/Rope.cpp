#include "Rope.h"

#include <iostream>

bool gp::engine::Rope::collision(Vector3f &collisionNormal, Vector3f &collisionPoint1, Vector3f &collisionPoint2,
		float_t &interpenetrationDepth) const
{
	//We assumed that collisionPoint is the center of the respective object.
	//To be changed in HardConstraint.h in a later worksheet
	collisionPoint1 = computeCollisionPoint1();
	collisionPoint2 = computeCollisionPoint2();

	//for now, we assume that ropes are attached to the centers of the objects
	interpenetrationDepth = (collisionPoint1 - collisionPoint2).norm() - m_length;

	if(interpenetrationDepth > 0){

		//Now from o2 to o1 , because the sign criteria used in CollisionResolver
		collisionNormal = (collisionPoint1 - collisionPoint2).normalized();
		return true;

	}
	return false;
}
