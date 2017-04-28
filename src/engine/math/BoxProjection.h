#ifndef GP_ENGINE_MATH_BOXPROJECTION_H
#define GP_ENGINE_MATH_BOXPROJECTION_H

#include <cmath>

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
private:
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
	 * @param box1HalfSize Half size of box 1
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
	{ }

	/**
	 * The overlap of the two boxes on a certain <code>axis</code>.
	 *
	 * @return value > 0 for an overlap, value < 0 for no overlap
	 */
	float_t overlapOnAxis(const gp::engine::Vector3f axis)
	{
		// TODO
		return 0;
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
		return 0;
	}
};

}

}

#endif // GP_ENGINE_MATH_BOXPROJECTION_H