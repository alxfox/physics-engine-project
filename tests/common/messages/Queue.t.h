#include <cxxtest/TestSuite.h>

#include <pthread.h>
#if defined(_WIN32) || defined(WIN32)
#include <windows.h>

/// http://stackoverflow.com/questions/5801813/c-usleep-is-obsolete-workarounds-for-windows-mingw
void usleep(__int64 usec) 
{ 
    HANDLE timer; 
    LARGE_INTEGER ft; 

    ft.QuadPart = -(10*usec); // Convert to 100 nanosecond interval, negative value indicates relative time

    timer = CreateWaitableTimer(NULL, TRUE, NULL); 
    SetWaitableTimer(timer, &ft, 0, NULL, NULL, 0); 
    WaitForSingleObject(timer, INFINITE); 
    CloseHandle(timer); 
}
#else
#include <unistd.h>
#endif

#include "common/messages/Queue.h"
#include "common/messages/Message.h"

/**
 * Struct counting available instances
 */
struct Count {
	static int count;

	Count() { count++; }
	Count(const Count&) { count++; } // Copy constructor
	virtual ~Count() { count--; }
};

int Count::count = 0;

struct T : Count {
	int t;
	T() : t(0) {}
	T(int t) : t(t) {}
};

class QueueTest : public CxxTest::TestSuite
{
public:
	void testSize_Empty(void)
	{
		gp::messages::Queue<T> queue;
		TS_ASSERT_EQUALS(queue.size(), 0);
		TS_ASSERT(queue.empty());

		queue.push(T());
		TS_ASSERT_EQUALS(queue.size(), 1);
		TS_ASSERT(!queue.empty());

		queue.push(T());
		TS_ASSERT_EQUALS(queue.size(), 2);
		TS_ASSERT(!queue.empty());

		queue.pop();
		TS_ASSERT_EQUALS(queue.size(), 1);
		TS_ASSERT(!queue.empty());

		queue.push(T());
		TS_ASSERT_EQUALS(queue.size(), 2);
		TS_ASSERT(!queue.empty());

		queue.pop();
		TS_ASSERT_EQUALS(queue.size(), 1);
		TS_ASSERT(!queue.empty());

		queue.pop();
		TS_ASSERT_EQUALS(queue.size(), 0);
		TS_ASSERT(queue.empty());

		queue.push(T());
		TS_ASSERT_EQUALS(queue.size(), 1);
		TS_ASSERT(!queue.empty());

		queue.pop();
		TS_ASSERT_EQUALS(queue.size(), 0);
		TS_ASSERT(queue.empty());
	}

	void testPush_Pop(void)
	{
		gp::messages::Queue<T> queue;

		queue.push(T(1));
		queue.push(T(2));

		TS_ASSERT_EQUALS(queue.pop().t, 1);

		queue.push(T(3));

		TS_ASSERT_EQUALS(queue.pop().t, 2);
		TS_ASSERT_EQUALS(queue.pop().t, 3);
	}

	void testAlloc(void)
	{
		TS_ASSERT_EQUALS(Count::count, 0);
		{
			gp::messages::Queue<Count> queue;

			queue.push(T());
			queue.push(Count());
			TS_ASSERT_EQUALS(Count::count, 2);

			queue.pop();
			queue.pop();
		}
		TS_ASSERT_EQUALS(Count::count, 0);
	}

	void testThreads(void)
	{
		pthread_t thread;
		gp::messages::Queue<T> queue;

		TS_ASSERT_EQUALS(pthread_create(&thread, 0L, &sender, &queue), 0);

		for (unsigned int i = 0; i < 3; ++i) {
			// Wait until data is in the queue
			while (queue.empty())
				usleep(20);
			TS_ASSERT_EQUALS(queue.pop().t, i);
		}

		TS_ASSERT_EQUALS(pthread_join(thread, 0L), 0);
	}

	/**
	 * Sender thread
	 */
	static void* sender(void *q)
	{
		gp::messages::Queue<T> &queue = *static_cast<gp::messages::Queue<T>*>(q);

		for (unsigned int i = 0; i < 3; ++i) {
			if (i == 1)
				// Sleep some time to make threads interleaving
				usleep(500);
			queue.push(T(i));
		}

		return 0L;
	}
};
