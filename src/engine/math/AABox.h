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
		// TODO
		return Vector3f();
	}
};

}

}

#endif // GP_ENGINE_MATH_AABOX_H