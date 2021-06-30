#ifndef GP_ENGINE_MATH_SKEWSYMMETRICMATRIX_H
#define GP_ENGINE_MATH_SKEWSYMMETRICMATRIX_H

#include "engine/utils.h"

namespace gp
{

namespace engine
{

/**
 * @brief Functions for skew-symmetric matrices
 */
class SkewSymmetricMatrix
{
public:
	/**
	 * @param vector The vector for creating the skew-symmetric matrix
	 * @param[out] matrix The resulting skew-symmetric matrix
	 */
	static void createSkewSymmetricMatrix(const Vector3f &vector, Matrix3f &matrix)
	{
		matrix.setZero();
		
		matrix(0,1) = -vector.z();
		matrix(0,2) = vector.y();
		matrix(1,0) = vector.z();
		matrix(1,2) = -vector.x();
		matrix(2,0) = -vector.y();
		matrix(2,1) = vector.x();
	}
};

}

}

#endif // GP_ENGINE_MATH_SKEWSYMMETRICMATRIX_H