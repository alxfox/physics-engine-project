#ifndef GP_COMMON_MESSAGES_QUEUE_H
#define GP_COMMON_MESSAGES_QUEUE_H

#include <memory>
#include <vector>

#include <pthread.h>
#include "common/pthread_spin_lock_shim.h"

namespace gp
{

/**
 * @brief A message system for two threads
 */
namespace messages
{

/**
 * @brief A FIFO message queue for one sender and one receiver.
 *
 * The queue is implemented as two vectors:
 *
 * <pre>
 * m_out                               m_in
 * ---------------------------------   ---------------------------------
 * | 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 |   | 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 |
 * ---------------------------------   ---------------------------------
 *           ^                   ^                       ^
 *           |                   |                       |
 *         m_next          m_out.size()              m_in.size()
 * </pre>
 *
 * {@link pop()} will increase {@link m_next} until {@link m_out}.size()
 * is reached. Once {@link m_out} is "empty", {@link m_in} and {@link m_out}
 * will be switched.
 *
 * {@link push()} will add elements to {@link m_in}.
 *
 * The class is threadsafe for one sender and one receiver.
 */
template<typename T>
class Queue final
{
private:
	/** Vector for inserting elements */
	std::vector<std::unique_ptr<T>> *m_in;

	/** Vector for retrieving elements */
	std::vector<std::unique_ptr<T>> *m_out;

	/**
	 * @brief The current size of the in vector
	 *
	 * This needs to be volatile because the two threads might read/write
	 * in arbitrary order.
	 */
	volatile size_t m_sizeIn;

	/**
	 * Index of the next element in {@link m_out}.
	 *
	 * The index will grow if we remove elements, if the index is equal
	 * to <code>m_out.size()</code>, we can no longer pop from {@link m_out}
	 */
	size_t m_next;

	/** Spinlock for concurrent accesses */
	pthread_spinlock_t m_lock;

public:
	Queue()
		: m_in(new std::vector<std::unique_ptr<T>>()),
		m_out(new std::vector<std::unique_ptr<T>>()),
		m_sizeIn(0),
		m_next(0)
	{
		pthread_spin_init(&m_lock, PTHREAD_PROCESS_PRIVATE);
	}

	~Queue()
	{
		delete m_in;
		delete m_out;

		pthread_spin_destroy(&m_lock);
	}

	/**
	 * @return The current size of the queue
	 */
	size_t size() const
	{
		return m_sizeIn + m_out->size() - m_next;
	}

	/**
	 * @return <code>true</code> if the queue is empty
	 */
	bool empty() const
	{
		return size() == 0;
	}

	/**
	 * Pops the next element from the queue
	 *
	 * @return The next element
	 *
	 * @warning The behaviour is undefined if the queue is empty.
	 */
	const T& pop()
	{
		if (m_next >= m_out->size()) {
			m_out->clear();

			pthread_spin_lock(&m_lock);
			std::swap(m_in, m_out);
			m_sizeIn = 0;
			pthread_spin_unlock(&m_lock);

			m_next = 0;
		}

		return *((*m_out)[m_next++]);
	}

	/**
	 * If the queue is empty, busy waits for a new message.
	 * Otherwise it behaves like pop.
	 *
	 * @see {@link pop()}
	 */
	const T& waitAndPop()
	{
		while (empty())
			;

		return pop();
	}

	/**
	 * Adds (a copy of) the element to the queue
	 */
	template<typename TT>
	void push(const TT &element)
	{
		pthread_spin_lock(&m_lock);
		m_in->emplace_back(new TT(element));
		m_sizeIn++;
		pthread_spin_unlock(&m_lock);
	}
};

}

}

#endif // GP_COMMON_MESSAGES_QUEUE_H
