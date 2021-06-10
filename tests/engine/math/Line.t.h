#include <cxxtest/TestSuite.h>

#include "engine/math/Line.h"

class LineTest : public CxxTest::TestSuite
{
public:
	/**
	 * @todo Currently only two basic tests are implemented
	 */

		//bool rueckgabe = true;

	void testClosestPoints(void)
	{
		gp::engine::Line line1(gp::engine::Vector3f::Zero(), gp::engine::Vector3f(0, 1, 0));
		gp::engine::Line line2(gp::engine::Vector3f(1, 0, 0), gp::engine::Vector3f(0, 0, 1));

		gp::engine::Vector3f point1, point2;
		line1.closestPoints(line2, point1, point2);
		TS_ASSERT_EQUALS(point1, gp::engine::Vector3f::Zero());
		TS_ASSERT_EQUALS(point2, gp::engine::Vector3f(1, 0, 0));

		gp::engine::Line line3(gp::engine::Vector3f(0, 1, 1), gp::engine::Vector3f(0, 1, 1));
		gp::engine::Line line4(gp::engine::Vector3f(10, 1, -1), gp::engine::Vector3f(1, 0, 0));

		line3.closestPoints(line4, point1, point2);
		TS_ASSERT_EQUALS(point1, gp::engine::Vector3f::Zero());
		TS_ASSERT_EQUALS(point2, gp::engine::Vector3f(0, 1, -1));

		// two parallel lines
		gp::engine::Line line5(gp::engine::Vector3f::Zero(), gp::engine::Vector3f(1, 0, 0));
		gp::engine::Line line6(gp::engine::Vector3f(0, 1, 0), gp::engine::Vector3f(1, 0, 0));
		bool back = line5.closestPoints(line6, point1, point2);
		TS_ASSERT_EQUALS(back, false);
		
		//two intersecting lines
		gp::engine::Line line7(gp::engine::Vector3f::Zero(), gp::engine::Vector3f(1, 0, 0));
		gp::engine::Line line8(gp::engine::Vector3f(1, -1, 0), gp::engine::Vector3f(0, 1, 0));
		line7.closestPoints(line8, point1, point2);
		TS_ASSERT_EQUALS(point1, gp::engine::Vector3f(1,0,0));
		TS_ASSERT_EQUALS(point2, gp::engine::Vector3f(1,0,0));

		//normal working test
		gp::engine::Line line9(gp::engine::Vector3f(1,-1, 1), gp::engine::Vector3f(-1, 0, -1));
		gp::engine::Line line10(gp::engine::Vector3f(1, 1, -1), gp::engine::Vector3f(-1, 0, 1));
		TS_ASSERT_EQUALS(point1, gp::engine::Vector3f::Zero());
		TS_ASSERT_EQUALS(point2, gp::engine::Vector3f::Zero());
	}
};
