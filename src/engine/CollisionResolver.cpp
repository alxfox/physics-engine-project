#include <algorithm>
#include <iostream>

#include "CollisionResolver.h"
#include "math/SkewSymmetricMatrix.h"

void gp::engine::CollisionResolver::resolveInterpenetration()
{
	// TODO
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
