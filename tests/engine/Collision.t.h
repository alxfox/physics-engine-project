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

    void testdetectBoxBox(void)
	{
		gp::engine::Box box1(1., gp::engine::Vector3f(3.4f, 0.0f, 0.0f));
		gp::engine::Box box2(1.,gp::engine::Vector3f(4.0f, -0.5f, 0.0f));
		gp::engine::Object* o1 = &box1;
		gp::engine::Object* o2 = &box2;
		gp::engine::Collision coll(o1, o2);
		
		
	}
	private:
	static void helperVPBoxBox(const gp::engine::Box &box1,const gp::engine::Box &box2, gp::engine::Collision coll)
	{
		bool ret = coll.detect();
		//collisionNormal is normalized
		TS_ASSERT_DELTA(coll.collisionNormal().norm(),1.,gp::engine::EPSILON);

		gp::engine::Vector3f normal = coll.collisionPoint2()-coll.collisionPoint1();
		float_t interpen = normal.norm();
		//interpentration depth is distance between collision points
		TS_ASSERT_DELTA(coll.interpenetrationDepth(),interpen,gp::engine::EPSILON);

		//normal points from point 1 to point 2
		normal.normalize();
		TS_ASSERT_DELTA(coll.collisionNormal().x(),normal.x(),gp::engine::EPSILON);
		TS_ASSERT_DELTA(coll.collisionNormal().y(),normal.y(),gp::engine::EPSILON);
		TS_ASSERT_DELTA(coll.collisionNormal().z(),normal.z(),gp::engine::EPSILON);

		//one collision point is inside one box

		//one collision point is on a box plane
		gp::engine::Vector3f plane_b1p1 = box1.invModelMatrix()*coll.collisionPoint1();

		
		
	}
	static void helperEEBoxBox(const gp::engine::Box &box1,const gp::engine::Box &box2, gp::engine::Collision coll)
	{
		bool ret = coll.detect();
		//collisionNormal is normalized
		TS_ASSERT_DELTA(coll.collisionNormal().norm(),1.,gp::engine::EPSILON);

		gp::engine::Vector3f normal = coll.collisionPoint2()-coll.collisionPoint1();
		float_t interpen = normal.norm();
		//interpentration depth is distance between collision points
		TS_ASSERT_DELTA(coll.interpenetrationDepth(),interpen,gp::engine::EPSILON);

		//normal points from point 1 to point 2
		normal.normalize();
		TS_ASSERT_DELTA(coll.collisionNormal().x(),normal.x(),gp::engine::EPSILON);
		TS_ASSERT_DELTA(coll.collisionNormal().y(),normal.y(),gp::engine::EPSILON);
		TS_ASSERT_DELTA(coll.collisionNormal().z(),normal.z(),gp::engine::EPSILON);

		//both collision points are on an edge of a box
		

	}
};
