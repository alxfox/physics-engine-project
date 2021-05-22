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
	Vector3f moveDistance = m_collision.collisionNormal()*m_collision.interpenetrationDepth();
	// checking for isMovable is unnecessary when using invMass
	float_t m1 = obj1->invMass();
	float_t m2 = obj2->invMass();
	float_t d1 = m1/(m1+m2);
	float_t d2 = m2/(m1+m2);
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
	// TODO
	Object* obj1 = m_collision.object1();
	Object* obj2 = m_collision.object2();

	//Getting velocity, Cr and inverse masses from objects 
	Vector3f v1 = obj1->velocity();
	Vector3f v2 = obj2->velocity();
	float_t COF = fmin(obj1->restitutionCoefficient(), obj2->restitutionCoefficient());
	float_t invM1 = obj1->invMass();
	float_t invM2 = obj2->invMass();

	float_t massFraction1 = invM1/(invM1+invM2);
	float_t massFraction2 = invM2/(invM1+invM2);

	//Getting Vc just along collision normal direction

	Vector3f vC = (v1-v2).dot(m_collision.collisionNormal())*m_collision.collisionNormal();

	//Updating velocities
	Vector3f v1New = -(1 + COF) * massFraction1 * vC;
	Vector3f v2New = (1 + COF) * massFraction2 * vC;
	obj1->changeVelocity(v1New);
	obj2->changeVelocity(v2New);

	if(obj1->isMovable() && obj2->isMovable()) {
		Vector3f n = m_collision.collisionNormal();
		float_t Ekin1Before = obj1->mass()*v1.dot(v1)/2.0f;
		float_t Ekin2Before = obj2->mass()*v2.dot(v2)/2.0f;
		float_t Ekin1After = obj1->mass()*(v1New+v1).dot(v1New+v1)/2.0f;
		float_t Ekin2After = obj2->mass()*(v2New+v2).dot(v2New+v2)/2.0f;

		Vector3f momentumBefore1 = obj1->mass()*v1;
		Vector3f momentumBefore2 = obj2->mass()*v2;
		Vector3f momentumAfter1 = obj1->mass()*(v1New+v1);
		Vector3f momentumAfter2 = obj2->mass()*(v2New+v2);
		/*float_t beforeM = m_collision.collisionNormal().dot(obj1->mass()*v1 + obj2->mass()*v2);
		float_t afterM = m_collision.collisionNormal().dot(obj1->mass()*obj1->velocity() + obj2->mass()*obj2->velocity());
		float_t diffM = abs(beforeM-afterM);*/
		Vector3f mDiff =  (momentumAfter1+ momentumAfter2) - (momentumBefore1 + momentumBefore2); 
		
		float_t ourDiff = (Ekin1After + Ekin2After) - (Ekin1Before + Ekin2Before); 

		//Checking momentum conservation
		if(mDiff != engine::Vector3f::Ones()*0) {
		//	std::cout << mDiff << std::endl;
		}


		float_t theoryDiff = -((1.0f-COF*COF) / 2.0f);
		theoryDiff*=((v1-v2).dot(m_collision.collisionNormal()));
		theoryDiff*=((v1-v2).dot(m_collision.collisionNormal()));
		theoryDiff/=(invM1 + invM2);

		//Checking kinetic energy loss
		if (std::abs(ourDiff - theoryDiff) >= EPSILON) {
			std::cout << std::abs(ourDiff - theoryDiff) << "  " << EPSILON << std::endl;
		}
		//assert(std::abs(ourDiff - theoryDiff) < EPSILON);
	}else if (obj1->isMovable()){
		float_t beforeM = m_collision.collisionNormal().dot(obj1->mass()*v1);
		float_t afterM = m_collision.collisionNormal().dot(obj1->mass()*obj1->velocity());
		float_t diffM = COF*abs(beforeM)-abs(afterM);
		if(diffM>EPSILON)std::cout<<diffM<<std::endl;
	}else if (obj2->isMovable()){
		float_t beforeM = m_collision.collisionNormal().dot(obj2->mass()*v2);
		float_t afterM = m_collision.collisionNormal().dot(obj2->mass()*obj2->velocity());
		float_t diffM = COF*abs(beforeM)-abs(afterM);
		if(diffM>EPSILON)std::cout<<diffM<<std::endl;
	}else{
		//std::cout<<"both immovable"<<std::endl;
		}
}

void gp::engine::CollisionResolver::applyCollisionImpulseWithoutFriction()
{
	// TODO
}

void gp::engine::CollisionResolver::applyRealisticCollisionImpulse()
{
	// TODO
}
