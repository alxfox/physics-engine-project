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
	static void createSkeySymmetricMatrix(const Vector3f &vector, Matrix3f &matrix)
	{
		// TODO
		matrix.setZero();
		float_t a = vector(0);
		float_t b = vector(1);
		float_t c = vector(2);
		
		//matrix(0,0) = 0; matrix(0,1) = -c; matrix(0,2) = b; 
		//matrix(1,0) = c; matrix(1,1) = 0; matrix(1,2) = -a; 
		//matrix(2,0) = -b; matrix(2,1) = a; matrix(2,2) = 0;
		matrix << 	0, -c, b,
					c, 0, -a,
		     		-b, a, 0;
	}
};

}

}

#endif // GP_ENGINE_MATH_SKEWSYMMETRICMATRIX_H