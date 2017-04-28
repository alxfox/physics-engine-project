#ifndef GP_ENGINE_OBJECTS_BOX_H
#define GP_ENGINE_OBJECTS_BOX_H

#include "Object.h"

namespace gp
{

namespace engine
{

/**
 * A rectengular box
 */
class Box : public Object
{
private:
	/** Half size of the box in x, y, z */
	const Vector3f m_halfSize;

public:
	Box(float mass = 1, const Vector3f &position = Vector3f::Zero(), const Vector3f &size = Vector3f::Ones(),
		const Vector3f &velocity = Vector3f::Zero(), const Quaternion &rotation = Quaternion())
		: Object(mass, position, velocity, rotation, rotationalInertia(mass, size), 0.5*size.norm()),
		m_halfSize(0.5*size)
	{ }

	const Vector3f& halfSize() const
	{
		return m_halfSize;
	}

private:
	static Matrix3f rotationalInertia(float_t mass, const Vector3f &size)
	{
		Matrix3f rotationalInertia;
		rotationalInertia.setZero();

		// TODO

		return rotationalInertia;
	}
};

}

}

#endif // GP_ENGINE_OBJECTS_BOX_H