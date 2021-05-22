#include "Rope.h"

#include <iostream>

bool gp::engine::Rope::collision(Vector3f &collisionNormal, Vector3f &collisionPoint1, Vector3f &collisionPoint2,
		float_t &interpenetrationDepth) const
{
	// TODO
	//m_objectPointX is in objPointX space, so we need to bring to world to compare
	interpenetrationDepth = (computeCollisionPoint2() - computeCollisionPoint1()).norm() - m_length;

	if(interpenetrationDepth > -EPSILON){

		//Negative direction, because the sign criteria used in CollisionResolver
		//collisionNormal = -(m_object2.modelMatrix()*m_objPoint2 - m_object1.modelMatrix()*m_objPoint1).normalized();

		//Or equivalently, now collisionNormal goes from object2 to object1 
		// o2 -> o1, we want to take o1 closer to o2, that's why (-1) in CollisionResolver
		// o2 -> o1, we want to take o2 closer to o1, that's why (1) in CollisionResolver
		collisionNormal = (m_object1.modelMatrix()*m_objPoint1 - m_object2.modelMatrix()*m_objPoint2).normalized();

		//We assumed that collisionPoint is the center of the respective object.
		//To be changed in HardConstraint.h in a later worksheet
		collisionPoint1 = computeCollisionPoint1();
		collisionPoint2 = computeCollisionPoint2();
		return true;

	}
	return false;
}
