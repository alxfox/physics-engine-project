#include "Rope.h"

#include <iostream>

bool gp::engine::Rope::collision(Vector3f &collisionNormal, Vector3f &collisionPoint1, Vector3f &collisionPoint2,
		float_t &interpenetrationDepth) const
{
	
	interpenetrationDepth = (m_object1.modelMatrix()*m_objPoint1-m_object2.modelMatrix()*m_objPoint2).norm()-m_length;
	if(interpenetrationDepth > 0){
		collisionPoint1 = computeCollisionPoint1();
		collisionPoint2 = computeCollisionPoint2();
		//collision normal is inverted for ropes so that the changes are applied in the opposite direction of a direct collision
		// it connects the attachment points of the rope on the objects
		collisionNormal = (m_object1.modelMatrix()*m_objPoint1-m_object2.modelMatrix()*m_objPoint2).normalized();
		return true;
	}
	return false;
}
