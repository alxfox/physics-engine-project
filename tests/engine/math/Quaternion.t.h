#include <cxxtest/TestSuite.h>

#include "engine/math/Quaternion.h"

class QuaternionTest : public CxxTest::TestSuite
{
public:
	/**
	 * @todo Add test for more rotations
	 */
	void testRotate(void)
	{
		gp::engine::Quaternion q1;
		q1.rotate(M_PI * 0.3, gp::engine::Vector3f(0, 1, 1));
		Eigen::Quaternion<gp::engine::float_t> e1(Eigen::AngleAxis<gp::engine::float_t>(M_PI * 0.3,
				gp::engine::Vector3f(0, 1, 1).normalized()));
		TS_ASSERT_DELTA(q1.i(), e1.x(), gp::engine::EPSILON);
		TS_ASSERT_DELTA(q1.j(), e1.y(), gp::engine::EPSILON);
		TS_ASSERT_DELTA(q1.k(), e1.z(), gp::engine::EPSILON);
		TS_ASSERT_DELTA(q1.w(), e1.w(), gp::engine::EPSILON);

		q1.rotate(M_PI, gp::engine::Vector3f(1, 0, 0));
		e1 = Eigen::Quaternion<gp::engine::float_t>(Eigen::AngleAxis<gp::engine::float_t>(M_PI, gp::engine::Vector3f(1, 0, 0))) * e1;
		TS_ASSERT_DELTA(q1.i(), e1.x(), gp::engine::EPSILON);
		TS_ASSERT_DELTA(q1.j(), e1.y(), gp::engine::EPSILON);
		TS_ASSERT_DELTA(q1.k(), e1.z(), gp::engine::EPSILON);
		TS_ASSERT_DELTA(q1.w(), e1.w(), gp::engine::EPSILON);
	}

	/**
	 * @todo Add test for more rotations
	 */
	void testRotationMatrix(void)
	{
		gp::engine::Quaternion q1;
		gp::engine::Affine3f rm1;
		rm1.setIdentity();
		TS_ASSERT(q1.rotationMatrix().isApprox(rm1, gp::engine::EPSILON));

		gp::engine::Quaternion q2(M_PI * 0.25, gp::engine::Vector3f(0, 0, 5));
		gp::engine::Affine3f rm2(Eigen::AngleAxis<gp::engine::float_t>(M_PI * 0.25, gp::engine::Vector3f::UnitZ()));
		TS_ASSERT(q2.rotationMatrix().isApprox(rm2, gp::engine::EPSILON));

		gp::engine::Quaternion q3(M_PI * 0.33, gp::engine::Vector3f(1, 2, 5));
		gp::engine::Affine3f rm3(Eigen::AngleAxis<gp::engine::float_t>(M_PI * 0.33, gp::engine::Vector3f(1, 2, 5).normalized()));
		TS_ASSERT(q3.rotationMatrix().isApprox(rm3, gp::engine::EPSILON));
	}
};