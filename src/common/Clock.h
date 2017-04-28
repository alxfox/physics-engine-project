#ifndef GP_COMMON_CLOCK_H
#define GP_COMMON_CLOCK_H

#include <chrono>
#include <thread>

namespace gp
{

class Clock final
{
private:
	/** Start of the clock */
	std::chrono::steady_clock::time_point m_start;

	/** Interval of the clock */
	std::chrono::steady_clock::duration m_interval;

	/** Number of ticks since start */
	unsigned long m_ticks;
public:
	/**
	 * @param interval The interval of the clock in seconds
	 */
	void start(float interval = 1.0/60.0)
	{
		m_start = std::chrono::steady_clock::now();
		m_interval = std::chrono::duration_cast<std::chrono::steady_clock::duration>(std::chrono::duration<float>{interval});
		m_ticks = 0;
	}

	/**
	 * Wait for the next tick
	 */
	void waitTick()
	{
		m_ticks++;

		std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();

		// Only sleep if we not already missed the last tick
		if (now < m_ticks*m_interval + m_start)
			std::this_thread::sleep_for(m_ticks*m_interval + m_start - now);
	}
};

}

#endif // GP_COMMON_CLOCK_H