#ifndef GP_COMMON_VECTORCOMPONENTMANAGER_H
#define GP_COMMON_VECTORCOMPONENTMANAGER_H

#include <cassert>
#include <vector>

#include "ComponentManager.h"

namespace gp
{

/**
 * @brief {@link ComponentManager} that uses a vector of pointers to store components
 *
 * @tparam T The base type of the objects
 */
template<typename T>
class VectorComponentManager : public ComponentManager<T*>
{
private:
	/** The list of all objects */
	std::vector<T*> m_objects;

public:
	virtual ~VectorComponentManager()
	{
		for (typename std::vector<T*>::iterator it = m_objects.begin();
				it != m_objects.end(); ++it) {
			delete *it;
		}
	}

	/**
	 * @param entity The entity of the {@link EntityManager} for this object
	 * @param object A reference the object that should assicated with the entity
	 * @tparam TT The real type of the object
	 * @return A pointer to the new object in the {@link VectorComponentManager}.
	 */
	template<typename TT>
	T* add(Entity entity, const TT &object)
	{
		size_t id = ComponentManager<T*>::add(entity, new TT(object));
		return getInstance(id);
	}

	/**
	 * @brief Provides direct access to all objects
	 */
	const std::vector<T*>& objects() const
	{
		return m_objects;
	}

protected:
	/**
	 * @brief Adds the instance to the vector
	 *
	 * @note This function should only be called by the parent class.
	 */
	size_t addInstance(Entity, T* object)
	{
		assert(object);
		m_objects.push_back(object);
		return m_objects.size() - 1;
	}

	/**
	 * @param id A valid object identifier
	 * @return The object that corresponds to the given <code>id</code>
	 */
	T* getInstance(size_t id)
	{
		assert(id < m_objects.size());
		return m_objects[id];
	}
};

}

#endif // GP_COMMON_VECTORCOMPONENTMANAGER_H
