#ifndef GP_COMMON_SOA_H
#define GP_COMMON_SOA_H

#include <cstring>
#include "ForEach.h"
#include "Magic.h"

#define SOA_TO_SOA(N, STRUCT, X) decltype(STRUCT::X)* X = nullptr;
#define SOA_TYPES(STRUCT, X) decltype(STRUCT::X)
#define SOA_STRUCT_MEMBER_POINTER(N, STRUCT, X) template<> auto struct_member_pointer<STRUCT, N>() -> decltype(&STRUCT::X) { return &STRUCT::X; }
#define SOA_SOA_MEMBER_POINTERT(N, STRUCT, X) template<> auto soa_member_pointer<STRUCT, N>() -> decltype(&to_soa<STRUCT>::X) { return &to_soa<STRUCT>::X; }
#define SOA_UPGRADE_STRUCT(STRUCT, ...) namespace gp { namespace soa { \
                                          template<> struct to_soa<STRUCT> { \
                                            MAGIC_FOR_EACH(SOA_TO_SOA, STRUCT, __VA_ARGS__) \
                                          }; \
                                          template<> struct member_count<STRUCT> { static size_t const count = MAGIC_NARG(__VA_ARGS__); }; \
                                          template<> struct types<STRUCT> { \
                                            template<size_t Index> using type = typename get_type<Index, MAGIC_FOR_EACH_COMMA_SEPARATED(SOA_TYPES, STRUCT, __VA_ARGS__)>::type; \
                                          }; \
                                          MAGIC_FOR_EACH(SOA_STRUCT_MEMBER_POINTER, STRUCT, __VA_ARGS__) \
                                          MAGIC_FOR_EACH(SOA_SOA_MEMBER_POINTERT, STRUCT, __VA_ARGS__) \
                                        }}

namespace gp
{

/// Inspired by https://blog.molecular-matters.com/2013/10/22/implementing-a-semi-automatic-structure-of-arrays-data-container/
namespace soa
{

template<typename T> struct to_soa {};
template<typename T> struct member_count {};
template<typename T> struct types {};

/** Member pointer for SoA and structs */
template<typename T, typename Type> using add_member_pointer = Type T::*;
template<typename T, size_t Index> static add_member_pointer<T, typename types<T>::template type<Index>> struct_member_pointer();
template<typename T, size_t Index> static add_member_pointer<to_soa<T>, typename types<T>::template type<Index>*> soa_member_pointer();

template<size_t Index, typename... Types>
struct get_type {
	template<size_t Idx, typename Type, typename... Tps>
	struct helper {
		/** Recursive extraction of the type */
		using type = typename helper<Idx-1, Tps...>::type;
	};

	template<typename Type, typename... Tps>
	struct helper<0, Type, Tps...> {
		/** End of recursive extraction */
		using type = Type;
	};

	/** The type at <code>Index</code> */
	using type = typename helper<Index, Types...>::type;
};

template<typename T>
class soa1d : public to_soa<T>
{
private:
	/** The current size of the array */
	size_t m_size;

	/** The size of the currently allocate buffer for the arrays */
	size_t m_maxSize;

	template<size_t Index>
	struct Allocate {
		/**
		 * Reallocates one array and copies all the old values
		 */
		void operator()(soa1d<T>* soa) {
			typedef typename types<T>::template type<Index> type;
			auto memory = new type[soa->m_maxSize];
			auto& member = soa->*(soa_member_pointer<T, Index>());
			if (member != nullptr) {
				memcpy(memory, member, soa->m_size * sizeof(type));
				delete[] member;
			}
			member = memory;
		}
	};

	template<size_t INDEX>
	struct Delete {
		/**
		 * Deletes one component from one array
		 */
		void operator()(soa1d<T>* soa) {
			delete[] &*(soa->*(soa_member_pointer<T, INDEX>()));
		}
	};

	template<size_t Index>
	struct AddItem {
		/**
		 * Adds one component to one array
		 */
		void operator()(soa1d<T>* soa, T const& item) {
			*(soa->*(soa_member_pointer<T, Index>()) + soa->m_size) = item.*(struct_member_pointer<T, Index>());
		}
	};

	template<size_t Index>
	struct Swap {
		/**
		 * Swaps two components in one array
		 */
		void operator()(soa1d<T>* soa, size_t index1, size_t index2) {
			std::swap(*(soa->*(soa_member_pointer<T, Index>()) + index1), *(soa->*(soa_member_pointer<T, Index>()) + index2));
		}
	};

	template<size_t INDEX>
	struct ToAos {
		/**
		 * Copies one component from one array to the original struct
		 */
		void operator()(soa1d<T> const* soa, T* item, size_t index) {
			item->*(struct_member_pointer<T, INDEX>()) = *(soa->*(soa_member_pointer<T, INDEX>()) + index);
		}
	};

public:
	/** The growth factor if the array becomes full */
	size_t GrowthFactor = 2;

	/**
	 * @param maxSize The default buffer size of the array (has to be greater or equal to 1)
	 */
	soa1d(size_t maxSize = 1) : m_size(0), m_maxSize(maxSize) {
		for_each<Allocate, member_count<T>::count>(this);
	}

	~soa1d() {
		m_size = 0;
		m_maxSize = 0;
		for_each<Delete, member_count<T>::count>(this);
	}

	/**
	 * Adds a (copy of the) item to the array
	 */
	size_t add(T const& item) {
		if (m_size >= m_maxSize) {
			m_maxSize = GrowthFactor * m_maxSize;
			for_each<Allocate, member_count<T>::count>(this);
		}
		for_each<AddItem, member_count<T>::count>(this, item);
		return m_size++;
	}

	/**
	 * Swaps to elements
	 */
	void swap(size_t index1, size_t index2) {
		for_each<Swap, member_count<T>::count>(this, index1, index2);
	}

	/**
	 * Removes the element at <code>index</code>
	 */
	void removeAt(size_t index) {
		swap(index, m_size-1);
		--m_size;
	}

	/**
	 * @return (A copy of) the element at <code>index</code>
	 */
	T getAos(size_t index) const {
		T aos;
		for_each<ToAos, member_count<T>::count>(this, &aos, index);
		return aos;
	}

	/**
	 * @return The current size of the array
	 */
	size_t size() const {
		return m_size;
	}
};

}

}

#endif // GP_COMMON_SOA_H
