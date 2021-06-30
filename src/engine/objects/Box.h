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
		//size 0 = width, 1 = height, 2 = depth
		Matrix3f rotationalInertia;
		rotationalInertia.setZero();
		rotationalInertia.setIdentity();
		rotationalInertia(0,0) = (1.f/12.f)*mass*(size(1)*size(1)+size(2)*size(2)); //height
		rotationalInertia(1,1) = (1.f/12.f)*mass*(size(0)*size(0)+size(2)*size(2));	//width
		rotationalInertia(2,2) = (1.f/12.f)*mass*(size(0)*size(0)+size(1)*size(1));	//depth
		// TODO

		return rotationalInertia;
	}
};

}

}

#endif // GP_ENGINE_OBJECTS_BOX_H