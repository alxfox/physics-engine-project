#include <algorithm>
#include <iostream>

#include "CollisionResolver.h"
#include "math/SkewSymmetricMatrix.h"

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
}

void gp::engine::CollisionResolver::applyCollisionImpulseWithoutFriction()
{
	// TODO
}

void gp::engine::CollisionResolver::applyRealisticCollisionImpulse()
{
	// TODO
}
