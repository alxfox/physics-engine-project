#include <cxxtest/TestSuite.h>

#include "engine/math/AABox.h"

class AABoxTest : public CxxTest::TestSuite
{
public:
	void testClosestPointOnSurface(void)
	{
		gp::engine::Box box1;  // Box of size 1
		gp::engine::AABox aabox1(box1);

		TS_ASSERT_EQUALS(aabox1.closestPointOnSurface(gp::engine::Vector3f(1., 1., 1.)),
			gp::engine::Vector3f(.5, .5, .5));
		TS_ASSERT_EQUALS(aabox1.closestPointOnSurface(gp::engine::Vector3f(1., -.5, 1.)),
			gp::engine::Vector3f(.5, -.5, .5));
		TS_ASSERT_EQUALS(aabox1.closestPointOnSurface(gp::engine::Vector3f(.1, -1., .2)),
			gp::engine::Vector3f(.1, -.5, .2));

		gp::engine::Box box2(1., gp::engine::Vector3f::Zero(), gp::engine::Vector3f(2., 3., 4.));
		gp::engine::AABox aabox2(box2);

		TS_ASSERT_EQUALS(aabox2.closestPointOnSurface(gp::engine::Vector3f(1., 1., 1.)),
			gp::engine::Vector3f(1., 1., 1.));
		TS_ASSERT_EQUALS(aabox2.closestPointOnSurface(gp::engine::Vector3f(10., -.5, -5.)),
			gp::engine::Vector3f(1., -.5, -2.));
		TS_ASSERT_EQUALS(aabox2.closestPointOnSurface(gp::engine::Vector3f(.1, 2., -1.)),
			gp::engine::Vector3f(.1, 1.5, -1.));
	}
};
