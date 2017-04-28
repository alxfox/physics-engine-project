#ifndef GP_ENGINE_MATH_QUATERNION_H
#define GP_ENGINE_MATH_QUATERNION_H

#include <cassert>
#include <cmath>

#include "engine/utils.h"

namespace gp
{

namespace engine
{

/**
 * @brief Implemenation of quaternions
 */
class Quaternion final
{
private:
	float_t m_i;

	float_t m_j;

	float_t m_k;

	float_t m_w;

public:
	Quaternion()
		: m_i(0), m_j(0), m_k(0), m_w(1)
	{ }

	/**
	 * Constructs a Quaternion with an initial rotation at a certain axis
	 */
	Quaternion(float_t angle, const Vector3f &axis)
		: Quaternion()
	{
		rotate(angle, axis);
	}

private:
	Quaternion(float_t i, float_t j, float_t k, float_t w)
		: m_i(i), m_j(j), m_k(k), m_w(w)
	{ }

public:
	float_t i() const
	{
		return m_i;
	}

	float_t j() const
	{
		return m_j;
	}

	float_t k() const
	{
		return m_k;
	}

	float_t w() const
	{
		return m_w;
	}

	Quaternion operator*(const Quaternion &q) const
	{
		Quaternion p;

		// TODO

		return p;
	}

	Quaternion& operator*=(const Quaternion &q)
	{
		*this = *this * q;

		return *this;
	}

	/**
	 * Add a rotation around along a certain axis
	 *
	 * @param angle The angle in radians
	 * @param axis The rotation axis
	 */
	void rotate(float_t angle, const Vector3f &axis)
	{
		rotateNormalized(angle, axis.normalized());
	}

	/**
	 * Add a rotation around along a certain axis
	 *
	 * @param rotation The rotation axis. The length of the vector gives
	 *  the rotation angle in radians
	 */
	void rotate(const Vector3f &rotation)
	{
		float_t angle = rotation.norm();
		if (angle < EPSILON)
			return;

		rotateNormalized(angle, rotation * (1.0 / angle));
	}

	/**
	 * @return The rotation matrix for the quaternion
	 */
	Affine3f rotationMatrix() const
	{
		Affine3f rotationMatrix;
		rotationMatrix.setIdentity();

		// TODO

		return rotationMatrix;
	}

	/**
	 * Normalizes the quaternion
	 */
	void normalize()
	{
		float_t inv_length = 1.0 / std::sqrt(m_i*m_i + m_j*m_j + m_k*m_k + m_w*m_w);
		m_i *= inv_length;
		m_j *= inv_length;
		m_k *= inv_length;
		m_w *= inv_length;
	}

private:
	/**
	 * Add a rotation around along a certain axis
	 *
	 * @param angle The angle in radians
	 * @param axis The rotation axis. The vector has to be normalized
	 */
	void rotateNormalized(float_t angle, const Vector3f &axis)
	{
		assert(std::abs(axis.norm() - 1.) < EPSILON);

		float_t sinhalf = std::sin(angle * 0.5);

		Quaternion q(sinhalf * axis.x(), sinhalf * axis.y(), sinhalf * axis.z(),
			std::cos(angle * 0.5));

		*this = q * *this;
	}
};

}

}

inline
std::ostream& operator<<(std::ostream &co, const gp::engine::Quaternion &q)
{
	return co << "[" << q.i() << ", " << q.j() << ", " << q.k() << ", " << q.w() << "]";
}

#endif // GP_ENGINE_MATH_QUATERNION_H