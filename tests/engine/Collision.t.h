#include <cxxtest/TestSuite.h>

#include "engine/Collision.h"
class CollisonTest : public CxxTest::TestSuite
{
public:
// TODO: Expand testing of Collision
	void testDetectSphereSphere(void)
	{
        gp::engine::Sphere sphere(1., gp::engine::Vector3f(3.2f, 0.0f, 0.0f));
		gp::engine::Box box(1.,gp::engine::Vector3f(2.0f, -0.1f, 0.0f));
		gp::engine::Object* o1 = &sphere;
		gp::engine::Object* o2 = &box;
		gp::engine::Collision coll(o1, o2);
		bool ret = coll.detect();
		TS_ASSERT_EQUALS(ret,true);
		// collPoint is on the sphere
		TS_ASSERT_DELTA((coll.collisionPoint1()-sphere.position()).norm(),sphere.radius(),gp::engine::EPSILON);
	}
    void testdetectSphereBox(void)
	{
		gp::engine::Sphere sphere(1., gp::engine::Vector3f(3.4f, 0.0f, 0.0f));
		gp::engine::Box box(1.,gp::engine::Vector3f(4.0f, -0.5f, 0.0f));
		gp::engine::Object* o1 = &sphere;
		gp::engine::Object* o2 = &box;
		gp::engine::Collision coll(o1, o2);
		bool ret = coll.detect();
		TS_ASSERT_EQUALS(ret,true);
		// collPoint is on the sphere
		//std::cout<<"This is a Test "  << coll.collisionPoint1();
		TS_ASSERT_DELTA((coll.collisionPoint1()-sphere.position()).norm(),sphere.radius(),gp::engine::EPSILON);
		// interpentration depth is distance between collision points
		TS_ASSERT_DELTA((coll.collisionPoint2()-coll.collisionPoint1()).norm(),coll.interpenetrationDepth(),gp::engine::EPSILON);
		//collNormal is normalized
		TS_ASSERT_DELTA(coll.collisionNormal().norm(),1.,gp::engine::EPSILON);
		//TS_ASSERT(false);
	}
};
