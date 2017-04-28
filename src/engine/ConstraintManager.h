#ifndef GP_ENGINE_CONSTRAINTMANAGER_H
#define GP_ENGINE_CONSTRAINTMANAGER_H

#include "constraints/HardConstraint.h"
#include "common/VectorComponentManager.h"

namespace gp
{

namespace engine
{

/**
 * @brief Constraint manager for the scenario
 *
 * @todo The {@link ConstraintManager} is currently limited to {@link HardConstraint}s.
 */
class ConstraintManager : public VectorComponentManager<HardConstraint>
{
};

}

}

#endif // GP_ENGINE_CONSTRAINTMANAGER_H