#ifndef GP_ENGINE_CONSTRAINTS_ROPE_H
#define GP_ENGINE_CONSTRAINTS_ROPE_H

#include "HardConstraint.h"

namespace gp
{

namespace engine
{

class Rope final : public HardConstraint
{
private:
	/** The length of the rope */
	float_t m_length;

public:
	Rope(Object &object1, Object &object2, float_t length = 3,
			const Vector3f &objPoint1 = gp::engine::Vector3f::Zero(),
			const Vector3f &objPoint2 = gp::engine::Vector3f::Zero())
		: HardConstraint(object1, object2, objPoint1, objPoint2),
		m_length(length)
	{ }

	~Rope()
	{ }

	bool collision(Vector3f &collisionNormal, Vector3f &collisionPoint1, Vector3f &collisionPoint2,
		float_t &interpenetrationDepth) const;
};

}

}

#endif // GP_ENGINE_CONSTRAINTS_ROPE_H