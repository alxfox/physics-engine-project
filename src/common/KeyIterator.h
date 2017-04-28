#ifndef GP_COMMON_MAPITERATOR_H
#define GP_COMMON_MAPITERATOR_H

namespace gp
{

/**
 * A map iterator that allows only access to the key, but not
 * to the value of the elements
 *
 * @todo Add missing operators
 */
template<template <class, class, class ...> class Map, class Key, class T>
class ConstKeyIterator
{
private:
	typename
	Map<Key, T>::const_iterator m_iterator;

public:
	ConstKeyIterator(typename Map<Key, T>::const_iterator iterator)
		: m_iterator(iterator)
	{ }

	ConstKeyIterator operator++()
	{
		++m_iterator;
		return *this;
	}

	bool operator!=(const ConstKeyIterator<Map, Key, T> &other)
	{
		return m_iterator != other.m_iterator;
	}

	const Key& operator*() const
	{
		return m_iterator->first;
	}
};

}

#endif // GP_COMMON_MAPITERATOR_H