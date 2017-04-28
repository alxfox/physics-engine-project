#ifndef GP_COMMON_SOACOMPONENTMANAGER_H
#define GP_COMMON_SOACOMPONENTMANAGER_H

#include <cstring>
#include <limits>

#include "Entity.h"
#include "ComponentManager.h"
#include "SoA.h"

namespace gp
{
  template<typename T>
  class SoAComponentManager : public ComponentManager<unsigned int> {
  public:
    typedef ComponentManager<unsigned int>::EntityIterator EntityIterator;

    soa::soa1d<T> data;

    typedef unsigned int Instance;

    Instance add(Entity entity)
    {
      unsigned int id = ComponentManager::add(entity, end());
      return getInstance(id);
    }

    void swap(Instance inst1, Instance inst2) {
      pthread_spin_lock(&m_lock);

      m_map[ data.entity[inst1] ] = inst2;
      m_map[ data.entity[inst2] ] = inst1;
      data.swap(inst1, inst2);

      pthread_spin_unlock(&m_lock);
    }

    virtual Instance end() const
    {
      return std::numeric_limits<Instance>::max();
    }

  protected:
    unsigned int addInstance(Entity entity, Instance)
    {
      T component;
      memset(&component, 0, sizeof(T));
      component.entity = entity;
      data.add(component);
      return data.size() - 1;
    }

    Instance getInstance(unsigned int id)
    {
      return id;
    }
  };
}

#endif // GP_COMMON_SOACOMPONENTMANAGER_H
