#ifndef GP_COMMON_ENTITY_H
#define GP_COMMON_ENTITY_H

#include <inttypes.h>
#include <limits>
#include <functional>

namespace gp
{

uint64_t const ENTITY_INDEX_BITS = 24;
uint64_t const ENTITY_INDEX_MASK = (1 << ENTITY_INDEX_BITS)-1;

uint64_t const ENTITY_GENERATION_MASK = std::numeric_limits<uint64_t>::max() ^ ENTITY_INDEX_MASK;

/// See http://bitsquid.blogspot.de/2014/08/building-data-oriented-entity-system.html
struct Entity {
	uint64_t id;

	uint64_t index() const { return id & ENTITY_INDEX_MASK; }
	uint64_t generation() const { return (id >> ENTITY_INDEX_BITS) & ENTITY_GENERATION_MASK; }
};

}

namespace std
{

template<> struct hash<gp::Entity>
{
public:
	/**
	 * @return The hash code for an entity
	 */
	size_t operator()(gp::Entity const& e) const
	{
		return hash<uint64_t>()(e.id);
	}
};

template<> struct equal_to<gp::Entity>
{
public:
	/**
	 * @return <code>True</code> if the entities are equal
	 */
	bool operator()(gp::Entity const& a, gp::Entity const& b) const
	{
		return a.id == b.id;
	}
};

}

#endif // GP_COMMON_ENTITY_H
