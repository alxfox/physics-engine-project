#ifndef GP_ENGINE_MATH_BOXPROJECTION_H
#define GP_ENGINE_MATH_BOXPROJECTION_H

#include <cmath>
#include <cstddef>
#include <iostream>
#include <limits>

#include "engine/utils.h"

namespace gp
{

namespace engine
{

/**
 * @brief Helper function(s) to project boxes on lines
 */
class BoxProjection
{
public:
	const gp::engine::Vector3f * const m_box1Axis;
	const gp::engine::Vector3f &m_box1HalfSize;
	const gp::engine::Vector3f * const m_box2Axis;
	const gp::engine::Vector3f &m_box2HalfSize;
	const gp::engine::Vector3f &m_center2center;

public:
	/**
	 * To avoid redundent computations, this constructor take references to two {@link Box}es
	 * but uses precomputed data.
	 *
	 * @param box1Axis The 3 (normalized) axis of box one
	 * @param box1errorHalfSize Half size of box 1
	 * @param box2Axis The 3 (normalized) axis of box two
	 * @param box2HalfSize Half size of box 2
	 * @param center2center Vector pointing from center of box 1 to the center of box 2
	 */
	BoxProjection(const gp::engine::Vector3f *box1Axis, const gp::engine::Vector3f &box1HalfSize,
		const gp::engine::Vector3f box2Axis[3], const gp::engine::Vector3f &box2HalfSize,
		const gp::engine::Vector3f &center2center)
		: m_box1Axis(box1Axis), m_box1HalfSize(box1HalfSize),
		m_box2Axis(box2Axis), m_box2HalfSize(box2HalfSize),
		m_center2center(center2center)
	{
		//std::cout << "c2c \n"<< center2center<< std::endl;
		//std::cout << "c2c \n"<< m_center2center<< std::endl;
	 }

	/**
	 * The overlap of the two boxes on a certain <code>axis</code>.
	 *
	 * @return value > 0 for an overlap, value < 0 for no overlap
	 */
	float_t overlapOnAxis(const gp::engine::Vector3f axis)
	{
		float_t totalLength = abs(m_center2center.dot(axis));
		return projectToAxis(m_box1Axis,m_box1HalfSize,axis)+projectToAxis(m_box2Axis,m_box2HalfSize,axis)-totalLength;
	}

private:
	/**
	 * Project a box (with axis <code>boxAxis</code> and half size <code>halfSize</code>) to a given
	 * <code>axis</code>.
	 *
	 * @return The size of the projection of the box dived by 2
	 */
	static float_t projectToAxis(const gp::engine::Vector3f boxAxis[3], const gp::engine::Vector3f &boxHalfSize,
			const gp::engine::Vector3f axis)
	{
		// TODO
		//assert(abs(boxAxis[0].norm() - 1) < EPSILON);
		//assert(abs(boxAxis[1].norm() - 1) < EPSILON);
		//assert(abs(boxAxis[2].norm() - 1) < EPSILON);
		//assert(abs(axis.norm() - 1) < EPSILON);

		Vector3f diag1 = (boxAxis[0]*boxHalfSize[0]+boxAxis[1]*boxHalfSize[1]+boxAxis[2]*boxHalfSize[2]);
		Vector3f diag2 = (-boxAxis[0]*boxHalfSize[0]+boxAxis[1]*boxHalfSize[1]+boxAxis[2]*boxHalfSize[2]);
		Vector3f diag3 = (boxAxis[0]*boxHalfSize[0]-boxAxis[1]*boxHalfSize[1]+boxAxis[2]*boxHalfSize[2]);
		Vector3f diag4 = (boxAxis[0]*boxHalfSize[0]+boxAxis[1]*boxHalfSize[1]-boxAxis[2]*boxHalfSize[2]);
		float_t ret = std::max(abs(diag1.dot(axis)),abs(diag2.dot(axis)));
		ret = std::max(ret, abs(diag3.dot(axis)));
		ret = std::max(ret, abs(diag4.dot(axis)));
		return ret;
	}
};

}

}

#endif // GP_ENGINE_MATH_BOXPROJECTION_H
