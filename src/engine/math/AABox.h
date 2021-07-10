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
		if(!(abs(point(0)) > m_box.halfSize()(0) || abs(point(1)) > m_box.halfSize()(1) || abs(point(2)) > m_box.halfSize()(2)))
		std::cout<<"test"<<std::endl;
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
	bool collideRay(const Vector3f &point, const Vector3f &dir, Vector3f &ret){
		//for each axis check -1 and 1
		//x-axis
		bool foundOne = false;
		float_t lowestDistance = std::numeric_limits<float_t>::max();
		for(int axis = 0; axis<3;axis++){
			for(int i = -1; i<2;i+=2){
				float_t t = ((i) - point[axis])/dir[axis];
				if(abs(point[axis]+t*dir[axis] - (i))<EPSILON){
					Vector3f current = point+t*dir;
					//check if point is the one closest to the source so far
					float_t curDist = (point-current).norm();
					if(curDist < lowestDistance)
					{
						//point is on the plane and closest, next check if it's within the bounds of the box
						int ax1 = (axis+1)%3;
						int ax2 = (axis+2)%3;
						if(current[ax1]<=m_box.halfSize()(ax1) && current[ax1]>=-m_box.halfSize()(ax1) 
						&& current[ax2]<=m_box.halfSize()(ax2) && current[ax2]>=-m_box.halfSize()(ax2)){
							lowestDistance = curDist;
							ret.x()=current.x();
							ret.y()=current.y();
							ret.z()=current.z();
							foundOne = true;
						}
					}
				}
			}
		}
		return foundOne;
	}
};

}

}

#endif // GP_ENGINE_MATH_AABOX_H