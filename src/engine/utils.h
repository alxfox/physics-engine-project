#ifndef GP_ENGINE_UTILS_H
#define GP_ENGINE_UTILS_H

#include "Eigen/Core"
#include "Eigen/Geometry"

#include "config.h"

namespace gp
{

namespace engine
{

/** Vector of size 2 */
typedef Eigen::Matrix<float_t, 2, 1> Vector2f;

/** Vector of size 3 */
typedef Eigen::Matrix<float_t, 3, 1> Vector3f;

/** Vector of size 4 */
typedef Eigen::Matrix<float_t, 4, 1> Vector4f;

/** Matrix of size 2x2 */
typedef Eigen::Matrix<float_t, 2, 2> Matrix2f;

/** Matrix of size 3x3 */
typedef Eigen::Matrix<float_t, 3, 3> Matrix3f;

/** Matrix of size 4x4 */
typedef Eigen::Matrix<float_t, 4, 4> Matrix4f;

/** Affine transformation in 3D */
typedef Eigen::Transform<float, 3, Eigen::Affine> Affine3f;

/** Useful for assert macros in production builds */
#define _unused(x) ((void)(x))

}

}

#endif // GP_ENGINE_UTILS_H