#include <cxxtest/TestSuite.h>

#include "engine/math/SkewSymmetricMatrix.h"

class SkewSymmetricMatrixTest : public CxxTest::TestSuite
{
public:
	void testCreateSkewSymmetricMatrix(void)
	{
		gp::engine::Vector3f v1(1, 2, 3);
		gp::engine::Matrix3f m1;
		m1 << 0, -3, 2,
			3, 0, -1,
			-2, 1, 0;

		gp::engine::Matrix3f res;
		gp::engine::SkewSymmetricMatrix::createSkeySymmetricMatrix(v1, res);

		TS_ASSERT_EQUALS(res, m1);
	}
};