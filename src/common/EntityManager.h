#ifndef GP_COMMON_ENTITYMANAGER_H
#define GP_COMMON_ENTITYMANAGER_H

#include <cassert>
#include <queue>
#include <vector>

#include <pthread.h>
#include "pthread_spin_lock_shim.h"

#include "Entity.h"

namespace gp
{

/// See http://bitsquid.blogspot.de/2014/08/building-data-oriented-entity-system.html
class EntityManager final
{
private:
	std::vector<uint64_t> m_generation;
	std::queue<uint64_t> m_freeIndices;

	/** Spinlock for concurrent accesses to {@link create}/{@link destroy} */
	pthread_spinlock_t m_lock;

public:
	EntityManager()
	{
		pthread_spin_init(&m_lock, PTHREAD_PROCESS_PRIVATE);
	}

	~EntityManager()
	{
		pthread_spin_destroy(&m_lock);
	}

	Entity create()
	{
		pthread_spin_lock(&m_lock);

		uint64_t index;
		if (!m_freeIndices.empty()) {
			index = m_freeIndices.front();
			m_freeIndices.pop();
		} else {
			m_generation.push_back(0);
			index = m_generation.size() - 1;
			assert(index < (1 << ENTITY_INDEX_BITS));
		}

		Entity e;
		e.id = (index & ENTITY_INDEX_MASK) | (m_generation[index] << ENTITY_INDEX_BITS);

		pthread_spin_unlock(&m_lock);

		return e;
	}

	bool alive(Entity e) const
	{
		return m_generation[e.index()] == e.generation();
	}

	/**
	 * @brief Deletes the entity for the entity manager
	 *
	 * @note This function does not remove the entity from component managers.
	 */
	void destroy(Entity e)
	{

		if (alive(e)) {
			unsigned index = e.index();

			pthread_spin_lock(&m_lock);
			++m_generation[index];
			m_freeIndices.push(index);
			pthread_spin_unlock(&m_lock);
		}
	}
};

}

#endif // GP_COMMON_ENTITYMANAGER_H
