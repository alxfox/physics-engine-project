#include <cxxtest/TestSuite.h>

#include "engine/math/BoxProjection.h"
#include "engine/objects/Box.h"

class BoxProjectionTest : public CxxTest::TestSuite
{
public:
	/**
	 * @todo Currently only basic tests are implemented
	 */
	void testOverlapOnAxis(void)
	{
		gp::engine::Box box1;
		gp::engine::Box box2;

		gp::engine::Vector3f axes1[3];
		gp::engine::Vector3f axes2[3];

		boxAxes(box1, axes1);
		boxAxes(box2, axes2);

		gp::engine::BoxProjection boxProjection1(axes1, box1.halfSize(), axes2, box2.halfSize(),
			center2center(box1, box2));

		TS_ASSERT_DELTA(boxProjection1.overlapOnAxis(gp::engine::Vector3f(1, 0, 0)), 1., gp::engine::EPSILON);
		TS_ASSERT_DELTA(boxProjection1.overlapOnAxis(gp::engine::Vector3f(1, 1, 0).normalized()), std::sqrt(2.), gp::engine::EPSILON);

		gp::engine::Box box3(1, gp::engine::Vector3f(0.5, 0.5, 0.5));
		boxAxes(box3, axes2);

		gp::engine::BoxProjection boxProjection2(axes1, box1.halfSize(), axes2, box3.halfSize(),
			center2center(box1, box3));
		TS_ASSERT_DELTA(boxProjection2.overlapOnAxis(gp::engine::Vector3f(1, 0, 0)), .5, gp::engine::EPSILON);

		gp::engine::Box box4(1, gp::engine::Vector3f(0.5, 0.5, 1.), gp::engine::Vector3f(1, 0.5, 0.2));
		boxAxes(box4, axes2);

		gp::engine::BoxProjection boxProjection4(axes1, box1.halfSize(), axes2, box4.halfSize(),
			center2center(box1, box4));
		TS_ASSERT_DELTA(boxProjection4.overlapOnAxis(gp::engine::Vector3f(1, 0, 0)), .5, gp::engine::EPSILON);
		TS_ASSERT_DELTA(boxProjection4.overlapOnAxis(gp::engine::Vector3f(0, 1, 0)), .25, gp::engine::EPSILON);
		TS_ASSERT_LESS_THAN(boxProjection4.overlapOnAxis(gp::engine::Vector3f(0, 0, 1)), 0.);
	}

private:
	static void boxAxes(const gp::engine::Box &box, gp::engine::Vector3f axes[3])
	{
		axes[0] = box.modelMatrix().matrix().block<3, 1>(0, 0);
		axes[1] = box.modelMatrix().matrix().block<3, 1>(0, 1);
		axes[2] = box.modelMatrix().matrix().block<3, 1>(0, 2);
	}

	static gp::engine::Vector3f center2center(const gp::engine::Box &box1, const gp::engine::Box &box2)
	{
		return box2.position() - box1.position();
	}
};