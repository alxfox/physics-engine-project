#include <algorithm>
#include <iostream>

#include "CollisionResolver.h"
#include "math/SkewSymmetricMatrix.h"

void gp::engine::CollisionResolver::resolveInterpenetration()
{
	// TODO
	float_t depth = m_collision.interpenetrationDepth();
	Vector3f normal = m_collision.collisionNormal();
	float_t invM1 = m_collision.object1()->invMass();
	float_t invM2 = m_collision.object2()->invMass();
	float_t d1 = invM1 / (invM1 + invM2);
	float_t d2 = invM2 / (invM1 + invM2);

	//(-1) because normal points from o1 to o2
	m_collision.object1()->setPosition(m_collision.object1()->position() + (-1)*normal*depth*d1);

	m_collision.object2()->setPosition(m_collision.object2()->position() + (1)*normal*depth*d2);
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
