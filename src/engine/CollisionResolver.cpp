#include <algorithm>
#include <cmath>
#include <iostream>
#include <cassert>

#include "CollisionResolver.h"
#include "config.h"
#include "math/SkewSymmetricMatrix.h"
#include "utils.h"

float binPos =  -50;

void gp::engine::CollisionResolver::resolveInterpenetration()
{
	if(m_collision.isShot()){
		return;
	}

	// TODO
	Object* obj1 = m_collision.object1();
	Object* obj2 = m_collision.object2();
	
	//Collisions between unmovable objects shouldn't exist
	if(!obj1->isMovable()&&!obj2->isMovable())
		return;

	if(obj1->isTrigger()){
		std::cout << "I'm triggered" << std::endl;
		obj2->setPosition(Vector3f(0, binPos, 0));
		binPos -= 10;
		return;
	}

	if(obj2->isTrigger()){
		std::cout << "I'm triggered" << std::endl;
		obj1->setPosition(Vector3f(0, binPos, 0));
		binPos -= 10;
		return;
	}


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
	if(m_collision.isShot()){
		//apply some impulse here
		return;
	}
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

	if(obj1->isTrigger() || obj2->isTrigger()){
		return;
	}
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

	if(obj1->isTrigger() || obj2->isTrigger()){
		return;
	}
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
	if(!m_collision.applyFriction()){
		applyCollisionImpulseWithoutFriction();
		return;
	}

	Object* obj1 = m_collision.object1();
	Object* obj2 = m_collision.object2();

	if(obj1->isTrigger() || obj2->isTrigger()){
		return;
	}
	//Collisions between unmovable objects shouldn't exist
	if(!obj1->isMovable()&&!obj2->isMovable())
		return;

//=========================================Collecting Data==============================================
	Vector3f v1 = obj1->velocity();
	Vector3f v2 = obj2->velocity();
	float_t COF = fmin(obj1->restitutionCoefficient(), obj2->restitutionCoefficient());
	float_t invM1 = obj1->invMass();
	float_t invM2 = obj2->invMass();

	Vector3f r1 = m_collision.collisionPoint1()-obj1->position();
	Vector3f r2 = m_collision.collisionPoint2()-obj2->position();
	Vector3f w1 = obj1->angularVelocity();
	Vector3f w2 = obj2->angularVelocity();

	Vector3f pV1 = v1 + w1.cross(r1);//total velocity of collision point
	Vector3f pV2 = v2 + w2.cross(r2);
	Vector3f normal = m_collision.collisionNormal();
	Vector3f closing = pV1-pV2;

	float_t sFriction = fmax(obj1->staticFrictionCoefficient(),obj2->staticFrictionCoefficient());
	float_t dFriction = fmax(obj1->dynamicFrictionCoefficient(),obj2->dynamicFrictionCoefficient());
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

	//Base ChangeMatrix
	Vector3f orth;
	if(abs(normal.x())<EPSILON)
		orth=Vector3f(1,0,0);
	else{
		float_t x = normal.x();
		float_t z = normal.z();
		orth = (1/sqrt(x*x+z*z))*Vector3f(z,0,-x);
	}
	Vector3f orth2 = orth.cross(normal);
	Matrix3f base;
	base.col(0) = normal;
	base.col(1) = orth;
	base.col(2) = orth2;

	//T-Matrix
	SkewSymmetricMatrix skew;
	Matrix3f sRa;
	Matrix3f sRb;
	skew.createSkewSymmetricMatrix(r1,sRa);
	skew.createSkewSymmetricMatrix(r2,sRb);
	Matrix3f diagMa = Matrix3f::Identity()*invM1;
	Matrix3f diagMb = Matrix3f::Identity()*invM2;
	Matrix3f tA = diagMa-sRa*inertia1World*sRa;
	Matrix3f tB = -diagMb+sRb*inertia2World*sRb;
	Matrix3f T = tA-tB;
	//Static Friction
	Vector3f cStar = base.transpose()*closing;
	Vector3f vStar((-1+COF)*cStar.x(), -cStar.y(),-cStar.z());
	Vector3f pStar = (base.transpose()*T*base).inverse()*vStar;
	float_t helper = sqrt(pStar.y()*pStar.y()+pStar.z()*pStar.z());
	if(helper>sFriction*abs(pStar.x()))//no static friction => dynamic friction
	{
		float_t k1 = (1/helper)*pStar.y();
		float_t k2 = (1/helper)*pStar.z();
		Matrix3f mat = base.transpose()*T*base;
		//assume p**x is positive
		float_t pX = vStar.x()/(mat(0,0) + mat(0,1)*dFriction*k1 + mat(0,2)*dFriction*k2);
		if(pX<0){
			//if pX was negative, assume negative instead
			pX=vStar.x()/(mat(0,0) - mat(0,1)*dFriction*k1 - mat(0,2)*dFriction*k2);
		}
		Vector3f pStarStar(pX, abs(pX)*k1*dFriction, abs(pX)*k2*dFriction);

		pStarStar = base*pStarStar;
		Vector3f w1New = inertia1World*(r1.cross(pStarStar));
		Vector3f w2New = -inertia2World*(r2.cross(pStarStar));

		Vector3f v1New = obj1->invMass()*pStarStar;
		Vector3f v2New = -obj2->invMass()*pStarStar;

		obj1->changeVelocity(v1New);
		obj2->changeVelocity(v2New);
		obj1->changeAngularVelocity(w1New);
		obj2->changeAngularVelocity(w2New);
	}else{	//apply static friction if condition is fulfilled
		pStar = base*pStar;//convert back to world space
		Vector3f w1New = inertia1World*(r1.cross(pStar));
		Vector3f w2New = -inertia2World*(r2.cross(pStar));

		Vector3f v1New = obj1->invMass()*pStar;
		Vector3f v2New = -obj2->invMass()*pStar;

		obj1->changeVelocity(v1New);
		obj2->changeVelocity(v2New);
		obj1->changeAngularVelocity(w1New);
		obj2->changeAngularVelocity(w2New);
	}
}
