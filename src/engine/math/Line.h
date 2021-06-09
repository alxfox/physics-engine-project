#ifndef GP_ENGINE_MATH_LINE_H
#define GP_ENGINE_MATH_LINE_H

#include <cassert>

#include "engine/utils.h"

namespace gp
{

namespace engine
{

/**
 * @brief Useful functions for lines in 3D
 *
 * Lines are represented by a point a direction vector
 */
class Line
{
private:
	/** A point on the line */
	Vector3f m_point;

	/** Direction of the line */
	Vector3f m_direction;

public:
	Line(const Vector3f &point, const Vector3f &direction)
		: m_point(point), m_direction(direction)
	{ }

	/**
	 * @brief Computes the closest point of two lines
	 *
	 * <pre>
	 *                  / v
	 *                 /
	 * P_0     P(s_c) /       u
	 * -*-------*---------------------
	 *          |   /
	 *      w_c |  /
	 *          | /
	 *          |/ Q(t_c)
	 *          *
	 *         /
	 *    Q_0 *
	 * </pre>
	 *
	 * Algorithm:
	 * @f$ P_0 @f$, @f$ Q_0 @f$ are the two points on line 1 and line 2 respectively. @f$ u @f$, @f$ v @f$
	 * denote the directions of the lines.
	 *
	 * @f$ P(s_c) @f$ and @f$ Q(t_c) @f$ are the points we are looking for on line 1 and 2 respectively.
	 *
	 * Thus,
	 * @f[ w_c = P(s_c) - Q(t_c) = P_0 + s_c * u - Q_0 - t_c * v = w_0 + s_c * u - t_c * v @f]
	 * with @f$ w_0 = P_0 - Q_0 @f$.
	 *
	 * Since @f$ w_c @f$ has to be perpendicular to @f$ u @f$ and @f$ v @f$, the following two equations
	 * have to be fulfilled:
	 * @f[ dot(u, w_c) = 0 @f] and @f[ dot(v, w_c) = 0 @f]
	 *
	 * Inserting the equation for @f$w_c@f$, we get:
	 * @f[ dot(u, u) * s_c - dot(u, v) * t_c = - dot(u, w_0) @f]
	 * @f[ dot(v, u) * s_c - dot(v, v) * t_c = - dot(v, w_0) @f]
	 *
	 * Setting @f$ a = dot(u, u) @f$, @f$ b = dot(u, v) @f$, @f$ c = dot(v, v) @f$, @f$ d = dot(u, w_0) @f$
	 * and @f$ e = dot(v, w_0) @f$,we get @f$ s_c @f$ and @f$ t_c @f$:
	 *
	 * @f[ s_c = (be - cd) / (ac - bb) @f]
	 * @f[ t_c = (ae - bd) / (ac - bb) @f]
	 *
	 * See http://geomalgorithms.com/a07-_distance.html for a detailed graphical description.
	 *
	 * @param other The line to which we should find the closest point
	 * @param[out] pointOnLine1 The closest point on line 1
	 * @param[out] pointOnLine2 The closest point on line 2
	 * @return False if lines are parallel, true otherwise
	 */
	bool closestPoints(const Line &other, Vector3f &pointOnLine1, Vector3f &pointOnLine2)
	{
		//geting both lines u and v
		Vector3f u = m_direction;
		Vector3f v = other.m_direction;
		Vector3f P_0 = m_point;
		Vector3f Q_0 = other.m_point;

		//looking if lines are parallel
		if (abs(u.x()-v.x() < EPSILON) 
			&& abs(u.y()-v.y() < EPSILON)
			&& abs(u.z()- v.z() < EPSILON))
		{
			//if parallel return false
			return false;
		}
		
		//w_0 = P_0 - Q_0
		Vector3f w_0 = P_0 - Q_0;

		//getting closest points on u and v 
		float_t a = u.dot(u);
		float_t b = u.dot(v);
		float_t c = v.dot(v);
		float_t d = u.dot(w_0);
		float_t e = v.dot(w_0);
		float_t s_c = (b*e - c*d) / (a*c - b*b);
		float_t t_c = (a*e - b*d) / (a*c - b*b);
		pointOnLine1 = P_0 + u*s_c;
		pointOnLine2 = Q_0 + v*t_c;

		//w_c = w_0 + s_c * u - t_c * v
		Vector3f w_c = w_0 + s_c * u - t_c * v;





		return true;
	}
};

}

}

#endif // GP_ENGINE_MATH_LINE_H