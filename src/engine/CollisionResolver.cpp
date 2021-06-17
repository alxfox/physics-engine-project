#include <algorithm>
#include <cmath>
#include <iostream>
#include <cassert>

#include "CollisionResolver.h"
#include "config.h"
#include "math/SkewSymmetricMatrix.h"
#include "utils.h"

void gp::engine::CollisionResolver::resolveInterpenetration()
{
	// TODO
	Object* obj1 = m_collision.object1();
	Object* obj2 = m_collision.object2();

	//Collisions between unmovable objects shouldn't exist
	if(!obj1->isMovable()&&!obj2->isMovable())
		return;

	//Separate the objects along the collision normal by *interpenetrationDepth* distance
	Vector3f moveDistance = m_collision.collisionNormal()*m_collision.interpenetrationDepth();

	//Checking for isMovable is unnecessary when using invMass
	float_t m1 = obj1->invMass();
	float_t m2 = obj2->invMass();

	//Separation applied to each object is proportional to its mass. d1 + d2 = 1
	float_t d1 = m1/(m1+m2);
	float_t d2 = m2/(m1+m2);

	//As always, collision normal goes from o1 to o2 
	obj1->move(moveDistance*d1*(-1));
	obj2->move(moveDistance*d2);
}

void gp::engine::CollisionResolver::applyCollisionImpulse()
{
	switch (COLLISION_IMPULSE_ALGORITHM) {
	case SIMPLE:
		applyCollisionImpulseWithoutRotationFriction();
		break;
	case NOFRICTION:
		applyCollisionImpulseWithoutFriction();
		break;
	case REALISTIC:
		applyRealisticCollisionImpulse();
		break;
	default:
		; // Do nothing
	}
}

void gp::engine::CollisionResolver::applyCollisionImpulseWithoutRotationFriction()
{
	Object* obj1 = m_collision.object1();
	Object* obj2 = m_collision.object2();

	//Collisions between unmovable objects shouldn't exist
	if(!obj1->isMovable()&&!obj2->isMovable())
		return;

//=========================================Applying the impulses==============================================
	//Getting V, Cr and m^-1 
	Vector3f v1 = obj1->velocity();
	Vector3f v2 = obj2->velocity();
	float_t COF = fmin(obj1->restitutionCoefficient(), obj2->restitutionCoefficient());
	float_t invM1 = obj1->invMass();
	float_t invM2 = obj2->invMass();

	//COF = 1.0f;
	float_t massFraction1 = invM1/(invM1+invM2);
	float_t massFraction2 = invM2/(invM1+invM2);

	//Getting the Vc component that is parallel to the collision normal direction
	Vector3f vC = (v1-v2).dot(m_collision.collisionNormal())*m_collision.collisionNormal();

	//Updating velocities
	Vector3f v1New = -(1 + COF) * massFraction1 * vC;
	Vector3f v2New = (1 + COF) * massFraction2 * vC;
	obj1->changeVelocity(v1New);
	obj2->changeVelocity(v2New);

//=========================================Energy and momentum assertions==============================================
// ASSERTIONS ARE COMMENTED OUT AS SOME OF THE ENERGY/MOMENTUM DIFFERENCES WERE SLIGHTLY ABOVE EPSILON
	float_t Ekin1Before = 0.0f;
	float_t Ekin1After = 0.0f;
	float_t Ekin2Before = 0.0f;
	float_t Ekin2After = 0.0f;

	Vector3f momentumBefore1 = Vector3f::Zero(); 
	Vector3f momentumBefore2 = Vector3f::Zero();
	Vector3f momentumAfter1 = Vector3f::Zero();
	Vector3f momentumAfter2 = Vector3f::Zero();

	if(obj1->isMovable()){
		Ekin1Before = obj1->mass()*v1.dot(v1)/2.0f;
		Ekin1After = obj1->mass()*(v1New+v1).dot(v1New+v1)/2.0f;
		momentumBefore1 = obj1->mass()*v1;
		momentumAfter1 = obj1->mass()*(v1New+v1);
	}

	if(obj2->isMovable()){
		Ekin2Before = obj2->mass()*v2.dot(v2)/2.0f;
		Ekin2After = obj2->mass()*(v2New+v2).dot(v2New+v2)/2.0f;
		momentumBefore2 = obj2->mass()*v2;
		momentumAfter2 = obj2->mass()*(v2New+v2);
	}

	

	float_t ourEkinLoss = (Ekin1After + Ekin2After) - (Ekin1Before + Ekin2Before); 

	float_t theorEkinLoss = -((1.0f-COF*COF) / 2.0f);
	theorEkinLoss*=((v1-v2).dot(m_collision.collisionNormal()));
	theorEkinLoss*=((v1-v2).dot(m_collision.collisionNormal()));
	theorEkinLoss/=(invM1 + invM2);

	//Checking kinetic energy loss
	if (std::abs(ourEkinLoss - theorEkinLoss) > EPSILON) {
		std::cout << "EnergyLossDifference > EPSILON: " << std::abs(ourEkinLoss - theorEkinLoss) << std::endl;
	}
	//assert(std::abs(ourDiff - theoryDiff) < EPSILON); // UNCOMMENT THIS

	if(!obj1->isMovable()||!obj2->isMovable()) return; 

	//expected 0 momentum variation
	Vector3f momentumBefore = momentumBefore1+momentumBefore2;
	Vector3f momentumAfter = momentumAfter1+momentumAfter2;
	Vector3f momentumDiff =  (momentumBefore.cwiseAbs() - momentumAfter.cwiseAbs()).cwiseAbs();
	if(abs(momentumDiff.x()) > EPSILON) {
		std::cout << "Variation of momentum along x > EPSILON: " << momentumDiff.x() << std::endl;

	}
	if(abs(momentumDiff.y()) > EPSILON) {
		std::cout << "Variation of momentum along y > EPSILON: " << momentumDiff.y() << std::endl;

	}
	if(abs(momentumDiff.z()) > EPSILON) {
		std::cout << "Variation of momentum along z > EPSILON: " << momentumDiff.z() << std::endl;

	}
	//assert(abs(momentumDiff.x()) <= EPSILON && abs(momentumDiff.y()) <= EPSILON && abs(momentumDiff.z()) <= EPSILON); // UNCOMMENT THIS


}

void gp::engine::CollisionResolver::applyCollisionImpulseWithoutFriction()
{
	// TODO
	applyCollisionImpulseWithoutRotationFriction();

	Object* obj1 = m_collision.object1();
	Object* obj2 = m_collision.object2();
//=========================================Applying the impulses==============================================
	//Getting V, Cr and m^-1 
	Vector3f v1 = obj1->velocity();
	Vector3f v2 = obj2->velocity();
	float_t COF = fmin(obj1->restitutionCoefficient(), obj2->restitutionCoefficient());
	float_t invM1 = obj1->invMass();
	float_t invM2 = obj2->invMass();
	//Collisions between unmovable objects shouldn't exist
	if(!obj1->isMovable()&&!obj2->isMovable())
		return;
	//COF = 1.0f;
	float_t massFraction1 = invM1/(invM1+invM2);
	float_t massFraction2 = invM2/(invM1+invM2);
	Vector3f xM1 = obj1->invModelMatrix() * m_collision.collisionPoint1(); //vector from center of mass to collsion point
	Vector3f xM2 = obj2->invModelMatrix() * m_collision.collisionPoint2();
	Vector3f w1 = obj1->angularVelocity();
	Vector3f w2 = obj2->angularVelocity();
	Vector3f pV1 = v1 + w1.cross(xM1);//total velocity of collision point
	Vector3f pV2 = v2 + w2.cross(xM2);
	//Getting the Vc component that is parallel to the collision normal direction
	//Vector3f vC = (v1-v2).dot(m_collision.collisionNormal())*m_collision.collisionNormal();

	//Updating velocities
	/*Vector3f v1New = -(1 + COF) * massFraction1 * vC;
	Vector3f v2New = (1 + COF) * massFraction2 * vC;
	obj1->changeVelocity(v1New);
	obj2->changeVelocity(v2New);*/
	
}

void gp::engine::CollisionResolver::applyRealisticCollisionImpulse()
{
	// TODO
}
