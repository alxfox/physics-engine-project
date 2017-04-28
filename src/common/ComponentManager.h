#ifndef GP_COMMON_COMPONENTMANAGER_H
#define GP_COMMON_COMPONENTMANAGER_H

#include <unordered_map>

#include <pthread.h>
#include "pthread_spin_lock_shim.h"

#include "Entity.h"
#include "KeyIterator.h"

namespace gp
{

/**
 * Base class for component manager
 *
 * The component manager handles a certain manifestations of entities.
 *
 * @see Entity
 * @see EntityManager
 */
template<class Instance>
class ComponentManager
{
public:
	typedef ConstKeyIterator<std::unordered_map, Entity, unsigned int> EntityIterator;

protected:
	/** Map from entities to local instance ids */
	std::unordered_map<Entity, unsigned int> m_map;

	/** Spinlock for concurrent accesses */
	pthread_spinlock_t m_lock;

	ComponentManager()
	{
		pthread_spin_init(&m_lock, PTHREAD_PROCESS_PRIVATE);
	}

	virtual ~ComponentManager()
	{
		pthread_spin_destroy(&m_lock);
	}

public:
	/**
	 * @return An iterator pointing to the beginning of the entities
	 */
	EntityIterator entitiesBegin() const
	{
		return EntityIterator(m_map.cbegin());
	}

	/**
	 * @return An iterator pointing to an invalid entity
	 */
	EntityIterator entitiesEnd() const
	{
		return EntityIterator(m_map.cend());
	}

	/**
	 * Get the instance for an entity
	 *
	 * @return The instance for the entity or {@link end()}
	 */
	Instance find(Entity entity)
	{
		pthread_spin_lock(&m_lock);

		const auto e = m_map.find(entity);

		Instance inst;
		if (e == m_map.cend())
			inst = end();
		else
			inst = getInstance(e->second);

		pthread_spin_unlock(&m_lock);

		return inst;
	}

	/**
	 * @return An invalid instance
	 */
	virtual Instance end() const
	{
		return Instance(0);
	}

protected:
	/**
	 * Add an instance for an entity
	 *
	 * Redefine this function in the subclass to provide the correct
	 * parameters.
	 *
	 * @param instance The new instance.
	 * @return The storage id of the new object
	 */
	unsigned int add(Entity entity, const Instance instance)
	{
		pthread_spin_lock(&m_lock);
		unsigned int id = addInstance(entity, instance);
		m_map[entity] = id;
		pthread_spin_unlock(&m_lock);
		return id;
	}

	/**
	 * Adds a new instances.
	 *
	 * This function is called by {@link add} and should be implemented
	 * by subclasses to store the instance.
	 *
	 * The subclass is responsible for freeing the memory once the object
	 * is deleted from the manager.
	 *
	 * @param instance The new instance, will never be NULL
	 * @return The storage id of the instance
	 */
	virtual unsigned int addInstance(Entity entity, Instance instance) = 0;

	/**
	 * Get the instance for an id
	 *
	 * This function is called by {@link lookup} and should be implemented
	 * by subclasses.
	 */
	virtual Instance getInstance(unsigned int id) = 0;
};

}

#endif // GP_COMMON_COMPONENTMANAGER_H
