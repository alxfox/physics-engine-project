#ifndef GP_SCENARIOS_H
#define GP_SCENARIOS_H

#include "Scenario.h"

namespace gp
{

/**
 * @brief Standard arena
 */
class StandardArena : public Scenario
{
public:
	StandardArena();
};

/**
 * @brief First example sceenario
 */
class BoxBoxTest : public StandardArena
{
public:
	BoxBoxTest();
};

/**
 * @brief First example sceenario
 */
class Scenario1 : public StandardArena
{
public:
	Scenario1();
};

/**
 * @brief A lot of spheres
 */
class ManySpheres : public StandardArena
{
public:
	ManySpheres();
};

/**
 * @brief A lot of cubes
 */
class ManyCubes : public StandardArena
{
public:
	ManyCubes();
};

/**
 * @brief Hanging bridge with ropes
 */
class HangingBridge : public StandardArena
{
public:
	HangingBridge();
};

/**
 * @brief Pendulum
 */
class Pendulum : public Scenario
{
public:
	Pendulum();
};

}

#endif // GP_SCENARIOS_H
