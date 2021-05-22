#include <cassert>
#include <cmath>
#include <limits>
#include <iostream>
#include <ostream>

#include "Collision.h"
#include "config.h"
#include "math/AABox.h"
#include "math/BoxProjection.h"
#include "math/Line.h"
#include "math/utils.h"
#include "utils.h"

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
	Sphere* sphere1 = dynamic_cast<Sphere*>(m_object1);
	Sphere* sphere2 = dynamic_cast<Sphere*>(m_object2);
	float_t collDistance = sphere1->radius() + sphere2->radius(); //if distance is smaller than this value then there is a collision
	Vector3f collNormal = sphere2->position() - sphere1->position(); //normal vector from sphere1 to sphere2
	float_t collNormalLength = collNormal.norm();
	if(collNormalLength < collDistance){
		m_collisionNormal = collNormal.normalized();
		assert(abs(m_collisionNormal.norm() - 1) < EPSILON);
		m_collisionPoint1 = sphere1->position()+collNormal.normalized()*sphere1->radius();
		m_collisionPoint2 = sphere2->position()-collNormal.normalized()*sphere2->radius();
		m_interpenetrationDepth=collDistance-collNormalLength;
		return true;
	}
	return false;
}

bool gp::engine::Collision::detectSphereBox()
{
	Box* myBox= dynamic_cast<Box*>(m_object2);
	Sphere* mySphere = dynamic_cast<Sphere*>(m_object1);

	//if(myBox->isMovable()) {
	//	std::cout << "box: " << myBox->mass() << "  sphere: " << mySphere->mass() << std::endl;
	//	std::cout << "box: " << myBox->restitutionCoefficient() << "  sphere: " << mySphere->restitutionCoefficient() << std::endl;
	//}

	AABox aabox (*myBox);
	Vector3f sphereLocation = myBox->invModelMatrix() * mySphere->position(); // transform coordinates of the sphere in world space to the box's model space
	Vector3f boxSurfacePoint = aabox.closestPointOnSurface(sphereLocation); //!!is this correct?
	Vector3f collNormal = myBox->modelMatrix().linear()*(boxSurfacePoint-sphereLocation); // normal goes from the sphere center to the surface point of the box
	float_t collNormalLength = collNormal.norm();
	//collNormal.normalize();
	collNormal = (collNormal).normalized();
	if(collNormalLength < mySphere->radius()){
		//convert everything back to world space
		m_collisionPoint1 = myBox->modelMatrix()*sphereLocation + mySphere->radius()*collNormal;
		m_collisionPoint2 = myBox->modelMatrix()*boxSurfacePoint;
		m_collisionNormal = collNormal; //why do we not have to convert this back to worldspace?
		m_interpenetrationDepth = mySphere->radius()-collNormalLength;

		//Just assert that the distance from the center of the sphere to the plane, is bigger or equal than the radius
		Vector3f collisionPointToCenter = mySphere->position() - m_collisionPoint1;
		float_t distSpherePlane = abs(m_collisionNormal.dot(collisionPointToCenter));
		assert(distSpherePlane >= mySphere->radius()-EPSILON);
		//if(m_collisionNormal.x()<0.707106709+EPSILON&&m_collisionNormal.x()>0.707106709-EPSILON)
		//std::cout<<"test";
		//if(m_collisionNormal.x()<0.999879122+EPSILON&&m_collisionNormal.x()>0.999879122-EPSILON)
		//std::cout<<"test";
		//Just assert that the 8 corners of a box lie on the same side
		int pointsAbove = 0;
		int pointsBelow = 0;
		for (int i = -1; i < 2; i+=2){
			for (int j = -1; j < 2; j+=2){
				for (int k = -1; k < 2; k+=2){
					//Vector3f boxCorner = Vector3f(myBox->position().x()+ i*myBox->halfSize().x(), myBox->position().y()+j*myBox->halfSize().y(), myBox->position().z()+k*myBox->halfSize().z());
					Vector3f boxCorner = Vector3f(i*myBox->halfSize().x(), j*myBox->halfSize().y(), k*myBox->halfSize().z());
					boxCorner = myBox->modelMatrix()*boxCorner;
					Vector3f collisionPointToCorner = boxCorner - m_collisionPoint2;
					float_t dist = collisionPointToCorner.dot(m_collisionNormal);
					if(dist >= -EPSILON) {
						pointsAbove+=1;
					}
					if(dist < EPSILON) {
						pointsBelow+=1;
					}
				}
			}
		}
		if(!(pointsBelow==8 || pointsAbove==8))
			std::cout<<"warning";
		assert(pointsBelow==8 or pointsAbove==8);
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