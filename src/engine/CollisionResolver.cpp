#include <algorithm>
#include <cmath>
#include <iostream>

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
	float_t COF1 = obj1->restitutionCoefficient();
	float_t COF2 = obj2->restitutionCoefficient();
	float_t invM1 = obj1->invMass();
	float_t invM2 = obj2->invMass();

	float_t massFraction1 = invM1/(invM1+invM2);
	float_t massFraction2 = invM2/(invM1+invM2);

	float_t vC1 = (-m_collision.collisionNormal()).dot(v1);
	float_t vC2 = (-m_collision.collisionNormal()).dot(v2);
	//Vector3f vC = v1*vC1 - v2*vC2;
	Vector3f vC = (vC1-vC2)*m_collision.collisionNormal();

	Vector3f v1New = -(1 + COF1) * massFraction1 * vC;
	Vector3f v2New = (1 + COF2) * massFraction2 * vC;

	obj1->changeVelocity(v1New);
	obj2->changeVelocity(v2New);



}

void gp::engine::CollisionResolver::applyCollisionImpulseWithoutFriction()
{
	// TODO
}

void gp::engine::CollisionResolver::applyRealisticCollisionImpulse()
{
	// TODO
}
