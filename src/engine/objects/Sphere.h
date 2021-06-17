#ifndef GP_ENGINE_OBJECTS_SPHERE_H
#define GP_ENGINE_OBJECTS_SPHERE_H

#include "Object.h"

namespace gp
{

namespace engine
{

/**
 * A rectengular box
 */
class Sphere : public Object
{
private:
	/** Radius of the sphere */
	const float_t m_radius;

public:
	Sphere(float mass = 1, const Vector3f &position = Vector3f::Zero(), float_t radius = 1.f,
		const Vector3f &velocity = Vector3f::Zero(), const Quaternion &rotation = Quaternion())
		: Object(mass, position, velocity, rotation, rotationalInertia(mass, radius), radius),
		m_radius(radius)
	{ }

	float_t radius() const
	{
		return m_radius;
	}

private:
	static Matrix3f rotationalInertia(float_t mass, float_t radius)
	{
		Matrix3f rotationalInertia;
		rotationalInertia.setZero();
		rotationalInertia.setIdentity();
		rotationalInertia *= (2.f/5.f)*mass*radius*radius;
		// TODO

		return rotationalInertia;
	}
};

}

}

#endif // GP_ENGINE_OBJECTS_SPHERE_H