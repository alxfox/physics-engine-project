#include <cxxtest/TestSuite.h>

#include "common/utils/typeid.h"

class TypeIdTest : public CxxTest::TestSuite
{
public:
	void testTypeId(void)
	{
		struct T1 { };
		struct T2 { };

		TS_ASSERT_EQUALS(gp::type_id<T1>(), gp::type_id<T1>());
		TS_ASSERT_DIFFERS(gp::type_id<T1>(), gp::type_id<T2>());
	}
};