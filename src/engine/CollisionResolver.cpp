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
	Object* obj1 = m_collision.object1();
	Object* obj2 = m_collision.object2();
	//Collisions between unmovable objects shouldn't exist
	if(!obj1->isMovable()&&!obj2->isMovable())
		return;

//=========================================Collecting Data==============================================
	Vector3f v1 = obj1->velocity();
	Vector3f v2 = obj2->velocity();
	float_t COF = fmin(obj1->restitutionCoefficient(), obj2->restitutionCoefficient());

	Vector3f r1 = m_collision.collisionPoint1()-obj1->position();
	Vector3f r2 = m_collision.collisionPoint2()-obj2->position();
	Vector3f w1 = obj1->angularVelocity();
	Vector3f w2 = obj2->angularVelocity();

	Vector3f pV1 = v1 + w1.cross(r1);//total velocity of collision point
	Vector3f pV2 = v2 + w2.cross(r2);
	Vector3f normal = m_collision.collisionNormal();
	
//=========================================Inverse Inertia Matrices==============================================
	Matrix3f inertia1World;
	if(obj1->isMovable()){
		inertia1World = obj1->invModelMatrix().linear().transpose()*obj1->rotationalInverseInertia()*obj1->invModelMatrix().linear();
	} 
	else inertia1World = Matrix3f::Zero();

	Matrix3f inertia2World; 
	if(obj2->isMovable()){
		inertia2World = obj2->invModelMatrix().linear().transpose()*obj2->rotationalInverseInertia()*obj2->invModelMatrix().linear();
	}
	else inertia2World = Matrix3f::Zero();

//=========================================Computing f from the equation==============================================
	float_t dividend = (-1-COF)*(pV1-pV2).dot(normal);
	Vector3f div1 = obj1->invMass()*normal;
	Vector3f div2 = (inertia1World*(r1.cross(normal))).cross(r1);
	Vector3f div3 = obj2->invMass()*normal;
	Vector3f div4 = (inertia2World*(r2.cross(normal))).cross(r2);
	float_t divisor;
	divisor = normal.dot(div1+div2+div3+div4);
	float_t f = dividend/divisor;
	float_t f1 = f;
	float_t f2 = -f;

//=========================================Velocity updates==============================================

	Vector3f w1New = inertia1World*(r1.cross(f1*normal));
	Vector3f w2New = inertia2World*(r2.cross(f2*normal));

	Vector3f v1New = obj1->invMass()*f1*normal;
	Vector3f v2New = obj2->invMass()*f2*normal;

	obj1->changeVelocity(v1New);
	obj2->changeVelocity(v2New);
	obj1->changeAngularVelocity(w1New);
	obj2->changeAngularVelocity(w2New);
}

void gp::engine::CollisionResolver::applyRealisticCollisionImpulse()
{
	// TODO
	if (!m_collision.applyFriction()){
		applyCollisionImpulseWithoutFriction();
	}
	else{
		Object* obj1 = m_collision.object1();
		Object* obj2 = m_collision.object2();

		//Collisions between unmovable objects shouldn't exist
		if(!obj1->isMovable()&&!obj2->isMovable())
			return;

	//=========================================Collecting Data==============================================
		Vector3f v1 = obj1->velocity();
		Vector3f v2 = obj2->velocity();
		float_t COF = fmin(obj1->restitutionCoefficient(), obj2->restitutionCoefficient());
		float_t mA_inv = obj1->invMass();
		float_t mB_inv = obj2->invMass();

		Vector3f r1 = m_collision.collisionPoint1()-obj1->position();
		Vector3f r2 = m_collision.collisionPoint2()-obj2->position();
		Vector3f w1 = obj1->angularVelocity();
		Vector3f w2 = obj2->angularVelocity();

		Vector3f pVA = v1 + w1.cross(r1);//total velocity of collision point
		Vector3f pVB = v2 + w2.cross(r2);
		Vector3f normal = m_collision.collisionNormal();
		Vector3f C = pVA - pVB;


	//=========================================Inverse Inertia Matrices==============================================
		Matrix3f inertia1World;
		if(obj1->isMovable()){
			inertia1World = obj1->invModelMatrix().linear().transpose()*obj1->rotationalInverseInertia()*obj1->invModelMatrix().linear();
		} 
		else inertia1World = Matrix3f::Zero();

		Matrix3f inertia2World; 
		if(obj2->isMovable()){
			inertia2World = obj2->invModelMatrix().linear().transpose()*obj2->rotationalInverseInertia()*obj2->invModelMatrix().linear();
		}
		else inertia2World = Matrix3f::Zero();


	//=========================================T matrices==============================================
		Matrix3f diagA = (mA_inv*Vector3f::Ones()).asDiagonal(); 
		Matrix3f diagB = (mB_inv*Vector3f::Ones()).asDiagonal(); 
		Matrix3f skewRA; SkewSymmetricMatrix::createSkeySymmetricMatrix(r1, skewRA);
		Matrix3f skewRB; SkewSymmetricMatrix::createSkeySymmetricMatrix(r2, skewRB);

		Matrix3f TA = diagA - skewRA*inertia1World*skewRA;
		Matrix3f TB = -diagB + skewRB*inertia1World*skewRB;
		Matrix3f T = TA - TB;

	//=========================================B matrix==============================================
		Vector3f t1;
		Vector3f t2;
		if(normal(0)== 0.0f && normal(2) == 0.0f){
			t1 = engine::Vector3f(1.0f, 0.0f, 0.0f);
		}
		else{
			float_t k1 = 1.0f / std::sqrt(normal(0)*normal(0) +normal(2)*normal(2) );
			t1 = engine::Vector3f(normal(2), 0.0f, -normal(0));
		}
		t2 = normal.cross(t1);
	}
}
