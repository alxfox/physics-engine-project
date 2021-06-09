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
		
		//float_t box1ProjSize = projectToAxis(m_box1Axis, m_box1HalfSize, axis);
		//float_t box2ProjSize = projectToAxis(m_box2Axis, m_box2HalfSize, axis);
		//return box1ProjSize + box2ProjSize - abs(m_center2center.dot(axis));

		float_t maxProjectedCorner = -std::numeric_limits<float_t>::max();
		float_t minProjectedCorner = std::numeric_limits<float_t>::max();
		float_t maxProjectedCornerB1 = -std::numeric_limits<float_t>::max();
		float_t minProjectedCornerB1 = std::numeric_limits<float_t>::max();
		float_t maxProjectedCornerB2 = -std::numeric_limits<float_t>::max();
		float_t minProjectedCornerB2 = std::numeric_limits<float_t>::max();
		float_t projectedCorner1 = 0;
		float_t projectedCorner2 = 0;
		for (int i = -1; i < 2; i+=2){
			for (int j = -1; j < 2; j+=2){
				for (int k = -1; k < 2; k+=2){
					Vector3f boxCorner1 = i*m_box1Axis[0]*m_box1HalfSize.x() + j*m_box1Axis[1]*m_box1HalfSize.y() + k*m_box1Axis[2]*m_box1HalfSize.z();
					Vector3f boxCorner2 = m_center2center + i*m_box2Axis[0]*m_box2HalfSize.x() + j*m_box2Axis[1]*m_box2HalfSize.y() + k*m_box2Axis[2]*m_box2HalfSize.z();


					//std::cout << "bC2 \n"<< boxCorner2<< std::endl;
					//std::cout << "bC2 - c2c \n"<< boxCorner2 - m_center2center<< std::endl;
					//std::cout << "c2c \n"<< m_center2center<< std::endl;

					projectedCorner1 = boxCorner1.dot(axis);
					projectedCorner2 = boxCorner2.dot(axis);
					if(projectedCorner1 > maxProjectedCorner) {
						maxProjectedCorner = projectedCorner1;
					}
					if(projectedCorner1 > maxProjectedCornerB1) {
						maxProjectedCornerB1 = projectedCorner1;
					}
					if(projectedCorner2 > maxProjectedCorner) {
						maxProjectedCorner = projectedCorner2;
					}
					if(projectedCorner2 > maxProjectedCornerB2) {
						maxProjectedCornerB2 = projectedCorner2;
					}

					if(projectedCorner1 < minProjectedCorner) {
						minProjectedCorner = projectedCorner1;
					}
					if(projectedCorner1 < minProjectedCornerB1) {
						minProjectedCornerB1 = projectedCorner1;
					}
					if(projectedCorner2 < minProjectedCorner) {
						minProjectedCorner = projectedCorner2;
					}
					if(projectedCorner2 < minProjectedCornerB2) {
						minProjectedCornerB2 = projectedCorner2;
					}
				}
			}
		}
		//std::cout << maxProjectedCorner << " " << minProjectedCorner << std::endl;
		float_t totalProjSize = std::abs(maxProjectedCorner - minProjectedCorner);
		float_t b1ProjSize = std::abs(maxProjectedCornerB1 - minProjectedCornerB1);
		float_t b2ProjSize = std::abs(maxProjectedCornerB2 - minProjectedCornerB2);

		return b1ProjSize + b2ProjSize - totalProjSize;

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
		assert(abs(boxAxis[0].norm() - 1) < EPSILON);
		assert(abs(boxAxis[1].norm() - 1) < EPSILON);
		assert(abs(boxAxis[2].norm() - 1) < EPSILON);
		assert(abs(axis.norm() - 1) < EPSILON);

		float_t maxProjectedCorner = -std::numeric_limits<float_t>::max();
		float_t minProjectedCorner = std::numeric_limits<float_t>::max();
		float_t projectedCorner = 0;
		for (int i = -1; i < 2; i+=2){
			for (int j = -1; j < 2; j+=2){
				for (int k = -1; k < 2; k+=2){
					Vector3f boxCorner = i*boxAxis[0]*boxHalfSize.x() + j*boxAxis[1]*boxHalfSize.y() + k*boxAxis[2]*boxHalfSize.z();
					projectedCorner = boxCorner.dot(axis);
					if(projectedCorner > maxProjectedCorner) {
						maxProjectedCorner = projectedCorner;
					}
					if(projectedCorner < minProjectedCorner) {
						minProjectedCorner = projectedCorner;
					}
				}
			}
		}
		return std::abs(maxProjectedCorner - minProjectedCorner)/2.0f;
	}
};

}

}

#endif // GP_ENGINE_MATH_BOXPROJECTION_H