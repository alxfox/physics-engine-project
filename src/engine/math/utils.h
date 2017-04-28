#ifndef GP_ENGINE_MATH_UTILS_H
#define GP_ENGINE_MATH_UTILS_H

#include <cmath>

namespace gp
{

namespace engine
{

/**
 * Sign implementation
 */
template<typename T> inline
T sign(T v)
{
	return (T(0) < v) - (v < T(0));
}

/**
 * Simplified sign implementation for float and double
 *
 * @note In constrast to the mathematical signum definition, this
 *  function will not return 0.0 if v is +/- 0.0
 */
template<typename T> inline
T sign1(T v)
{
	return std::copysign(T(1.0), v);
}

}

}

#endif // GP_ENGINE_MATH_UTILS_H