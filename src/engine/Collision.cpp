#include <cassert>
#include <limits>
#include <iostream>
#include <ostream>

#include "Collision.h"
#include "math/AABox.h"
#include "math/BoxProjection.h"
#include "math/Line.h"
#include "math/utils.h"

bool gp::engine::Collision::detect()
{
	bool ret = false;


	switch (m_type) {
	case SPHERE_SPHERE:
		ret = detectSphereSphere();
		break;
	case SPHERE_BOX:
		ret = detectSphereBox();
		break;
	case BOX_BOX:
		ret = detectBoxBox();
		break;
	case CONSTRAINT:
		// Get collision data from the constraint
		assert(m_constraint);
		ret = m_constraint->collision(m_collisionNormal, m_collisionPoint1, m_collisionPoint2,
			m_interpenetrationDepth);
		break;
	}

	assert(!ret || !std::isnan(m_interpenetrationDepth));

	return ret;
}

bool gp::engine::Collision::detectSphereSphere()
{
	// TODO

	//Centers of the objects
	Vector3f p1 = m_object1->position();
	Vector3f p2 = m_object2->position();
	std::cout << "---------------" << std::endl;
	std::cout << p1 << std::endl;
	std::cout << p2 << std::endl;
	float_t d = (p1-p2).norm();

	//Segmentation fault in case one of the objects wouldn't be a Sphere
	float_t r1 = dynamic_cast<Sphere*>(m_object1)->radius();
	float_t r2 = dynamic_cast<Sphere*>(m_object2)->radius();
	if (d < (r1 + r2)){

		//distance if touching - real distance
		//m_interpenetrationDepth = (r1+r2) - d;
		
		//Unitary vector from o1 center to o2 center
		m_collisionNormal = (p2 - p1).normalized();
		assert(abs(m_collisionNormal.norm() - 1) < EPSILON);
		
		//From p1, direction collisionNormal, magnitude r1
		//Closest point to p2 that pertains to o1 
		m_collisionPoint1 = m_collisionNormal*r1 + p1;
		
		//From p2, (inverse sense) direction collisionNormal, magnitude r2
		//Closest point to p1 that pertains to o2 
		m_collisionPoint2 = -m_collisionNormal*r2 + p2;
		
		//or distance between collision points	
		m_interpenetrationDepth = (m_collisionPoint1 - m_collisionPoint2).norm();
		
		return true;
	}
	return false;
}

bool gp::engine::Collision::detectSphereBox()
{
	// TODO
	//Centers of the objects
	//First object Sphere, second object Box
	Vector3f scW = m_object1->position();
	Vector3f bcW = m_object2->position();

	float_t r1 = dynamic_cast<Sphere*>(m_object1)->radius();

	//Bring Sphere to Box space
	Vector3f scB = m_object2->invModelMatrix() * scW;
	//Vector3f p2b = m_object2->invModelMatrix() * p2;

	AABox b = AABox(*(dynamic_cast<Box*>(m_object2))); 

	Vector3f bs = b.closestPointOnSurface(scB);

	if ((bs - scB).norm() < r1) {
		m_collisionNormal = (bs - scB).normalized();
		m_collisionPoint1 = m_collisionNormal * r1 + scB;
		m_collisionPoint2 = bs;

		//bring back to World space
		m_collisionNormal = (m_object2->modelMatrix() * m_collisionNormal).normalized(); 
		m_collisionPoint1 = m_object2->modelMatrix() * m_collisionPoint1; 
		m_collisionPoint2 = m_object2->modelMatrix() * m_collisionPoint2; 
		m_interpenetrationDepth = (m_collisionPoint1 - m_collisionPoint2).norm();
		std::cout << "Collided!!!" << "\n" << m_collisionPoint1 << "\n" << m_collisionPoint2 << std::endl;
		return true;
	}
	return false;
}

bool gp::engine::Collision::detectBoxBox()
{
	// TODO
	return false;
}


gp::engine::Collision::CollisionType gp::engine::Collision::getType(gp::engine::Object *o1, gp::engine::Object *o2)
{
	if (dynamic_cast<Sphere*>(o1)) {
		if (dynamic_cast<Sphere*>(o2))
			return SPHERE_SPHERE;
		return SPHERE_BOX;
	} else {
		if (dynamic_cast<Sphere*>(o2))
			return SPHERE_BOX;
		return BOX_BOX;
	}
}

gp::engine::Object *gp::engine::Collision::getFirstObject(gp::engine::Object *o1, gp::engine::Object *o2)
{
	if (dynamic_cast<Sphere*>(o1))
		return o1;
	return o2;
}

gp::engine::Object *gp::engine::Collision::getSecondObject(gp::engine::Object *o1, gp::engine::Object *o2)
{
	if (dynamic_cast<Sphere*>(o1))
		return o2;
	return o1;
}