#ifndef GP_ENGINE_OBJECTMANAGER_H
#define GP_ENGINE_OBJECTMANAGER_H

#include "objects/Object.h"
#include "common/VectorComponentManager.h"

namespace gp
{

namespace engine
{

/**
 * @brief Object manager for scenarios
 *
 * The object manager manages als (physical) objects of a scenario.
 */
class ObjectManager : public VectorComponentManager<Object>
{
};

}

}

#endif // GP_ENGINE_OBJECTMANAGER_H