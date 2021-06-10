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

		gp::engine::Vector3f c2c;
		boxAxes(box1, axes1);
		boxAxes(box2, axes2);

		c2c = center2center(box1, box2);
		gp::engine::BoxProjection boxProjection1(axes1, box1.halfSize(), axes2, box2.halfSize(),
			c2c);

		TS_ASSERT_DELTA(boxProjection1.overlapOnAxis(gp::engine::Vector3f(1, 0, 0)), 1., gp::engine::EPSILON);
		TS_ASSERT_DELTA(boxProjection1.overlapOnAxis(gp::engine::Vector3f(1, 1, 0).normalized()), std::sqrt(2.), gp::engine::EPSILON);

		gp::engine::Box box3(1, gp::engine::Vector3f(0.5, 0.5, 0.5));
		boxAxes(box3, axes2);

		c2c = center2center(box1,box3);
		gp::engine::BoxProjection boxProjection2(axes1, box1.halfSize(), axes2, box3.halfSize(),
			c2c);
		TS_ASSERT_DELTA(boxProjection2.overlapOnAxis(gp::engine::Vector3f(1, 0, 0)), .5, gp::engine::EPSILON);

		gp::engine::Box box4(1, gp::engine::Vector3f(0.5, 0.5, 1.), gp::engine::Vector3f(1, 0.5, 0.2));
		boxAxes(box4, axes2);

		c2c = center2center(box1, box4);
		gp::engine::BoxProjection boxProjection4(axes1, box1.halfSize(), axes2, box4.halfSize(),
			c2c);

		TS_ASSERT_DELTA(boxProjection4.overlapOnAxis(gp::engine::Vector3f(1, 0, 0)), .5, gp::engine::EPSILON);
		TS_ASSERT_DELTA(boxProjection4.overlapOnAxis(gp::engine::Vector3f(0, 1, 0)), .25, gp::engine::EPSILON);
		TS_ASSERT_LESS_THAN(boxProjection4.overlapOnAxis(gp::engine::Vector3f(0, 0, 1)), 0.);

		gp::engine::Box box5(1, gp::engine::Vector3f(1., 1., 1.));
		boxAxes(box5, axes1);

		//No collision in the 3 axis (displacement to the right, up, and out)
		gp::engine::Box box6(1, gp::engine::Vector3f(2., 2., 2.));
		boxAxes(box6, axes2);
		c2c = center2center(box5,box6);
		gp::engine::BoxProjection boxProjection5(axes1, box5.halfSize(), axes2, box6.halfSize(),
			c2c);
		TS_ASSERT_DELTA(boxProjection5.overlapOnAxis(gp::engine::Vector3f(1, 0, 0)), 0., gp::engine::EPSILON);
		TS_ASSERT_DELTA(boxProjection5.overlapOnAxis(gp::engine::Vector3f(0, 1, 0)), 0., gp::engine::EPSILON);
		TS_ASSERT_DELTA(boxProjection5.overlapOnAxis(gp::engine::Vector3f(0, 0, 1)), 0., gp::engine::EPSILON);

		//No collision in 2 of the axis (displacement to the right and up)
		gp::engine::Box box7(1, gp::engine::Vector3f(2., 2., 1.));
		boxAxes(box7, axes2);
		c2c = center2center(box5,box7);
		gp::engine::BoxProjection boxProjection6(axes1, box5.halfSize(), axes2, box7.halfSize(),
			c2c);
		TS_ASSERT_DELTA(boxProjection6.overlapOnAxis(gp::engine::Vector3f(1, 0, 0)), 0., gp::engine::EPSILON);
		TS_ASSERT_DELTA(boxProjection6.overlapOnAxis(gp::engine::Vector3f(0, 1, 0)), 0., gp::engine::EPSILON);
		TS_ASSERT_DELTA(boxProjection6.overlapOnAxis(gp::engine::Vector3f(0, 0, 1)), 1., gp::engine::EPSILON);
		//No collision in 1 of axis (displacement to the right)
		gp::engine::Box box8(1, gp::engine::Vector3f(2., 1., 1.));
		boxAxes(box8, axes2);
		c2c = center2center(box5,box8);
		gp::engine::BoxProjection boxProjection8(axes1, box5.halfSize(), axes2, box8.halfSize(),
			c2c);
		TS_ASSERT_DELTA(boxProjection6.overlapOnAxis(gp::engine::Vector3f(1, 0, 0)), 0., gp::engine::EPSILON);
		TS_ASSERT_DELTA(boxProjection6.overlapOnAxis(gp::engine::Vector3f(0, 1, 0)), 1., gp::engine::EPSILON);
		TS_ASSERT_DELTA(boxProjection6.overlapOnAxis(gp::engine::Vector3f(0, 0, 1)), 1., gp::engine::EPSILON);


    	gp::engine::Box box9(1, gp::engine::Vector3f(1., 1., 1.),
  		gp::engine::Vector3f::Ones(),
   		gp::engine::Vector3f::Zero(),
		gp::engine::Quaternion(M_PI*0.25f, gp::engine::Vector3f(0, 0, 1)));
		boxAxes(box9, axes1);

		//One box rotated but no collision on projection axis 
		boxAxes(box7, axes2);
		c2c = center2center(box9,box7);
		gp::engine::BoxProjection boxProjection9(axes1, box9.halfSize(), axes2, box7.halfSize(),
			c2c);
		TS_ASSERT_LESS_THAN(boxProjection9.overlapOnAxis(gp::engine::Vector3f(0.7, 0.5, 0).normalized()), 0.);

		//One box rotated and collision on projection axis
		boxAxes(box7, axes2);
		c2c = center2center(box9,box7);
		gp::engine::BoxProjection boxProjection10(axes1, box9.halfSize(), axes2, box7.halfSize(),
			c2c);
		TS_ASSERT_DELTA(boxProjection10.overlapOnAxis(gp::engine::Vector3f(0, 0, 1).normalized()), 1., gp::engine::EPSILON);

		//2 box rotated but no collision on projection axis
    	gp::engine::Box box10(1, gp::engine::Vector3f(2., 2., 1.),
  		gp::engine::Vector3f::Ones(),
   		gp::engine::Vector3f::Zero(),
		gp::engine::Quaternion(M_PI*0.75f, gp::engine::Vector3f(0, 1, 0)));

		boxAxes(box10, axes2);
		c2c = center2center(box9,box10);
		gp::engine::BoxProjection boxProjection11(axes1, box9.halfSize(), axes2, box7.halfSize(),
			c2c);
		TS_ASSERT_LESS_THAN(boxProjection11.overlapOnAxis(gp::engine::Vector3f(1, 1, 0).normalized()), 0.);

		//2 box rotated and collision on projection axis
		TS_ASSERT_LESS_THAN(-boxProjection11.overlapOnAxis(gp::engine::Vector3f(0, 0, 1).normalized()), 0.);

		//almost no collision on projection axis, but a very little collision
		gp::engine::Box box12(1, gp::engine::Vector3f(1., 1., 1.));
		boxAxes(box12, axes1);

		gp::engine::Box box13(1, gp::engine::Vector3f(1.999, 2., 1.));
		boxAxes(box13, axes2);
		c2c = center2center(box12,box13);
		gp::engine::BoxProjection boxProjection12(axes1, box12.halfSize(), axes2, box13.halfSize(),
			c2c);

		TS_ASSERT_DELTA(boxProjection12.overlapOnAxis(gp::engine::Vector3f(1, 0, 0).normalized()), 0.001, gp::engine::EPSILON);

		//almost collision on projection axis, but no real collision
		gp::engine::Box box14(1, gp::engine::Vector3f(1., 1., 1.));
		boxAxes(box12, axes1);

		gp::engine::Box box15(1, gp::engine::Vector3f(2.001, 2., 1.));
		boxAxes(box13, axes2);
		c2c = center2center(box14,box15);
		gp::engine::BoxProjection boxProjection13(axes1, box12.halfSize(), axes2, box13.halfSize(),
			c2c);

		TS_ASSERT_LESS_THAN(boxProjection13.overlapOnAxis(gp::engine::Vector3f(1, 0, 0).normalized()), 0.);
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