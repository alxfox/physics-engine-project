#ifndef GP_ENGINE_MATH_AABOX_H
#define GP_ENGINE_MATH_AABOX_H

#include <cmath>
#include <iostream>

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
		//Point outside iff one of the coordinates is in absolute value bigger than corresponding half size
		assert(abs(point(0)) > m_box.halfSize()(0) || abs(point(1)) > m_box.halfSize()(1) || abs(point(2)) > m_box.halfSize()(2));

		//To project the point, we modify its components accordingly
		Vector3f projection(point);
		for (int i = 0; i < 3; i++) {
			//Respective component > Respective half size (we went further than halfSize)
			if (abs(projection(i)) > m_box.halfSize()(i) ) {
				//Project component to box
				// 							(1) or (-1)             	we cannot go further than halfSize      
				projection(i) = (projection(i) / abs(projection(i))) * m_box.halfSize()(i);
			}
		}
		return projection;
	}
};

}

}

#endif // GP_ENGINE_MATH_AABOX_H