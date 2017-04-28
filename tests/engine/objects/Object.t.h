#include <cxxtest/TestSuite.h>

#include "engine/objects/Box.h"

/**
 * Note: We use a box, since we cannot instanciate plain Objects.
 */
class BoxTest : public CxxTest::TestSuite
{
public:
	void testInvMass(void)
	{
		gp::engine::Box o0;
		TS_ASSERT_DELTA(o0.invMass(), 1.f, gp::engine::EPSILON);

		gp::engine::Box o1(42);
		TS_ASSERT_DELTA(o1.invMass(), 1.f/42.f, gp::engine::EPSILON);
	}

	void testMass(void)
	{
		gp::engine::Box o0;
		TS_ASSERT_DELTA(o0.mass(), 1.f, gp::engine::EPSILON);

		gp::engine::Box o1(42);
		TS_ASSERT_DELTA(o1.mass(), 42.f, gp::engine::EPSILON);
	}

	void testIsMovable(void)
	{
		gp::engine::Box o0;
		TS_ASSERT(o0.isMovable());

		gp::engine::Box o1(42);
		TS_ASSERT(o1.isMovable());

		gp::engine::Box o2(gp::engine::Object::UNMOVABLE_MASS);
		TS_ASSERT(!o2.isMovable());
	}

	void testPosition(void)
	{
		gp::engine::Box o0;
		TS_ASSERT_EQUALS(o0.position(), gp::engine::Vector3f::Zero());

		gp::engine::Box o1(1, gp::engine::Vector3f(1, 2, 3));
		TS_ASSERT_EQUALS(o1.position(), gp::engine::Vector3f(1, 2, 3));
	}

	void testSetPosition(void)
	{
		gp::engine::Box o0;
		o0.setPosition(gp::engine::Vector3f(5, 7, 8));
		TS_ASSERT_EQUALS(o0.position(), gp::engine::Vector3f(5, 7, 8));
	}

	void testMove(void)
	{
		gp::engine::Box o0;
		TS_ASSERT_EQUALS(o0.position(), gp::engine::Vector3f::Zero());
		o0.move(gp::engine::Vector3f(5, 7, 8));
		TS_ASSERT_EQUALS(o0.position(), gp::engine::Vector3f(5, 7, 8));

		o0.move(gp::engine::Vector3f(-3, -7, 1));
		TS_ASSERT_EQUALS(o0.position(), gp::engine::Vector3f(2, 0, 9));
	}

	void testVelocity(void)
	{
		gp::engine::Box o0;
		TS_ASSERT_EQUALS(o0.velocity(), gp::engine::Vector3f::Zero());

		gp::engine::Box o1(1, gp::engine::Vector3f::Zero(), gp::engine::Vector3f::Ones(), gp::engine::Vector3f(2, 3, 4));
		TS_ASSERT_EQUALS(o1.velocity(), gp::engine::Vector3f(2, 3, 4));
	}

	void testSetVelocity(void)
	{
		gp::engine::Box o0;
		o0.setVelocity(gp::engine::Vector3f(2.5, 3.5, 4.5));
		TS_ASSERT_EQUALS(o0.velocity(), gp::engine::Vector3f(2.5, 3.5, 4.5));
	}

	void testUpdateVelocity(void)
	{
		gp::engine::Box o0(1, gp::engine::Vector3f::Zero(), gp::engine::Vector3f::Ones(), gp::engine::Vector3f(2, -8, 10));
		TS_ASSERT_EQUALS(o0.velocity(), gp::engine::Vector3f(2, -8, 10));
		o0.updateVelocity(gp::engine::Vector3f(1, 0, 5), 1);
		TS_ASSERT_EQUALS(o0.velocity(), gp::engine::Vector3f(3, -8, 15));
		o0.updateVelocity(gp::engine::Vector3f(0, 1, -3), 0.5);
		TS_ASSERT_EQUALS(o0.velocity(), gp::engine::Vector3f(3, -7.5, 13.5));
	}

	void testUpdatePositionAndRotation(void)
	{
		gp::engine::Box o0(1, gp::engine::Vector3f::Zero(), gp::engine::Vector3f::Ones(), gp::engine::Vector3f(2, -8, 10));
		o0.setAngularVelocity(gp::engine::Vector3f::UnitX());
		o0.updatePositionAndRotation(gp::engine::Vector3f::UnitY() * 8.0, 0.5);
		TS_ASSERT_EQUALS(o0.position(), gp::engine::Vector3f(1, -3, 5));


		gp::engine::Quaternion q0(0.5, gp::engine::Vector3f::UnitX());
		TS_ASSERT_DELTA(o0.rotation().i(), q0.i(), gp::engine::EPSILON);
		TS_ASSERT_DELTA(o0.rotation().j(), q0.j(), gp::engine::EPSILON);
		TS_ASSERT_DELTA(o0.rotation().k(), q0.k(), gp::engine::EPSILON);
		TS_ASSERT_DELTA(o0.rotation().w(), q0.w(), gp::engine::EPSILON);
	}

	void testRestitutionCoefficient(void)
	{
		gp::engine::Box o0;
		TS_ASSERT_DELTA(o0.restitutionCoefficient(), 0.3, gp::engine::EPSILON);
	}

	void testSetRestitutionCoefficient(void)
	{
		gp::engine::Box o0;
		o0.setRestitutionCoefficient(2.3);
		TS_ASSERT_DELTA(o0.restitutionCoefficient(), 2.3, gp::engine::EPSILON);
	}

	// TODO test model matrix
};
