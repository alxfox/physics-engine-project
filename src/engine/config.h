#ifndef GP_ENGINE_CONFIG_H
#define GP_ENGINE_CONFIG_H

#include "Eigen/Core"

namespace gp
{

namespace engine
{

/** The precision used in the engine */
typedef float float_t;
//typedef double float_t;

/** The update interval of the engine */
const float_t UPDATE_INTERVAL = 1.0/60.0;

/** The slow motion factor (1 = real time, < 1 slower than real time) */
const float_t SLOW_MOTION_FACTOR = 0.2;

/** The constant acceleration (note: we cannot use Vector3f here) */
const Eigen::Matrix<float_t, 3, 1> ACCELERATION(0, -9.81*0, 0);

/** Number of collision resolve per time step */
const unsigned int COLLISION_RESOLVES_PER_STEP = 5;

/** Collision impulse algorithms */
enum CollisionImpulseType
{
	NOIMPULSE,
	SIMPLE,
	NOFRICTION,
	REALISTIC
};
/** Collision impulse algortihm in use */
const CollisionImpulseType COLLISION_IMPULSE_ALGORITHM = NOFRICTION;

/** Values below this threshold are considered zero */
const float_t EPSILON = 0.000001;


/**
 * In the case that the normal is computed by the 2 "intersection points"
 * (usually there's only one intersection point, but 2 on each object due
 * to the interpenetration), there's the problem that a normal cannot be
 * computed if the collision distance is very small and thus the
 * intersection points almost coincide. therefore a small "epsilon" is used
 * to overcome this limitation and allow a small interpenetration for such cases.
 *
 * this is not necessary for specific interpenetration cases - e. g.
 * the interpenetration between a sphere and a plane since the collision normal
 * is taken as the normal of a plane.
 *
 * however, using this restriction is necessary since the normal has to be
 * normalized by the collision distance. therefore a sufficient length
 * is unalterable.
 *
 * <b>MIN_COLLISION_DISTANCE has to be LARGER THAN ZERO!</b>
 */
const float_t MIN_COLLISION_DISTANCE = 0.00001;

}

}

#endif // GP_ENGINE_CONFIG_H
