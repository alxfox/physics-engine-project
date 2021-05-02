#ifndef GP_ENGINE_MATH_AABOX_H
#define GP_ENGINE_MATH_AABOX_H

#include <cmath>

#include "utils.h"
#include "engine/objects/Box.h"

namespace gp
{

namespace engine
{

/**
 * @brief Useful functions for axis-aligned boxes
 *
 * Helper function that assume an axis-aligned box at the center,
 * i.e. all functions ignore the position and rotation of the box
 */
class AABox
{
private:
	/** The "axis-aligned" box (position and rotation are ignored */
	const Box &m_box;

public:
	AABox(const Box &box)
		: m_box(box)
	{ }

	/**
	 * @return The closest point on the box surface to the given point
	 */
	Vector3f closestPointOnSurface(const Vector3f &point)
	{
		Vector3f closest = point;
		// TODO
		// assert that point is outside the box
		assert(!(abs(point.x())>m_box.halfSize().x() && abs(point.y())>m_box.halfSize().y() && abs(point.z())>m_box.halfSize().z()));
		
		// if our point is within the bounds of the box for two out of the three axis (as in it hovers over a face of the box) we just need to adjust the 3rd coordinate to the closest of either +h or -h of that axis 
		// this will project the point on that face
		/*if(abs(point.x()) < m_box.halfSize().x() && abs(point.y()) < m_box.halfSize().y()){
			closest.z() = m_box.halfSize().z() > 0  ? m_box.halfSize().z() : -m_box.halfSize().z();
		}*/

		// if our point is within the bounds of the box for one out of three axis we need to clamp the other two coordinates to the bounds of the box
		// the resulting point will be on one of the edges of the box
		/*else if(abs(point.x()) < m_box.halfSize().x()){
			closest.y() = m_box.halfSize().y() > 0  ? m_box.halfSize().y() : -m_box.halfSize().y();
			closest.z() = m_box.halfSize().z() > 0  ? m_box.halfSize().z() : -m_box.halfSize().z();
		}*/

		// if our point is out of the bounds of the box for all three axis we need to clamp all coordinates to the bounds of the box
		// the point will be on one of the corners
		/*else{

		}*/
		if(!(abs(point.x()) < m_box.halfSize().x())){
			closest.x() = m_box.halfSize().x() > 0  ? m_box.halfSize().x() : -m_box.halfSize().x();
		}
		if(!(abs(point.y()) < m_box.halfSize().y())){
			closest.y() = m_box.halfSize().y() > 0  ? m_box.halfSize().y() : -m_box.halfSize().y();
		}
		if(!(abs(point.z()) < m_box.halfSize().z())){
			closest.z() = m_box.halfSize().z() > 0  ? m_box.halfSize().z() : -m_box.halfSize().z();
		}

		return closest;
	}
};

}

}

#endif // GP_ENGINE_MATH_AABOX_H