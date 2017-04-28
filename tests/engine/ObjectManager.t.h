#include <cxxtest/TestSuite.h>

#include "engine/ObjectManager.h"
#include "engine/objects/Box.h"
#include "common/EntityManager.h"

/**
 * @todo Test the {@link VectorComponentManager} instead of the {@link ObjectManager}.
 */
class ObjectManagerTest : public CxxTest::TestSuite
{
public:
	void testFind(void)
	{
		gp::EntityManager manager;
		gp::engine::ObjectManager objectManager;

		gp::Entity e0 = manager.create();
		TS_ASSERT(!objectManager.find(e0));

		gp::Entity e1 = manager.create();
		TS_ASSERT(!objectManager.find(e1));

		gp::engine::Box b1(1, gp::engine::Vector3f(1, 2, 3));
		objectManager.add(e1, b1);
		TS_ASSERT(!objectManager.find(e0));
		TS_ASSERT(objectManager.find(e1));
		TS_ASSERT_DIFFERS(objectManager.find(e1), &b1);
		TS_ASSERT_EQUALS(objectManager.find(e1)->position(), b1.position());

		gp::Entity e2 = manager.create();
		TS_ASSERT(!objectManager.find(e2));
		TS_ASSERT_EQUALS(objectManager.find(e1)->position(), b1.position());

		gp::engine::Box b0(1, gp::engine::Vector3f(10, 20, 30));
		objectManager.add(e0, b0);
		TS_ASSERT(objectManager.find(e0));
		TS_ASSERT(objectManager.find(e1));
		TS_ASSERT_DIFFERS(objectManager.find(e0), &b0);
		TS_ASSERT_EQUALS(objectManager.find(e0)->position(), b0.position());
	}
};