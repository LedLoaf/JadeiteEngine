#include "timer.hpp"

using namespace std::chrono;

namespace jadeite::utilities
{
	
/* Start the time (start point steady_clock::now) */
void Timer::Start()
{
	if (!m_bIsRunning)
	{
		m_StartPoint = steady_clock::now();
		m_bIsRunning = true;
		m_bIsPaused = false;
	}
}

/* Stops the timer */
void Timer::Stop()
{
	if (m_bIsRunning)
	{
		m_bIsRunning = false;
	}
}

/* Pauses the timer until resumed */
void Timer::Pause()
{
	if (m_bIsRunning && !m_bIsPaused)
	{
		m_bIsPaused = true;
		m_PausedPoint = steady_clock::now();
	}
}

/* Resumes the timer when paused */
void Timer::Resume()
{
	if (m_bIsRunning && m_bIsPaused)
	{
		m_bIsPaused = false;
		m_StartPoint += duration_cast<milliseconds>(steady_clock::now() - m_PausedPoint);
	}
}

/* Restarts the clock all over */
void Timer::Restart()
{
	m_StartPoint = steady_clock::now();
	m_bIsRunning = true;
	m_bIsPaused = false;
}

/* Current elapsed time in milliseconds */
int64_t Timer::ElapsedMS() const
{
	if (m_bIsRunning)
	{
		if (m_bIsPaused)
		{
			return duration_cast<milliseconds>(m_PausedPoint - m_StartPoint).count();
		}
		else
		{
			return duration_cast<milliseconds>(steady_clock::now() - m_StartPoint).count();
		}
	}
	
	return 0;
}

/* Current elapsed time in seconds */
int64_t Timer::ElapsedSec() const
{
	return ElapsedMS() / 1000;
}

} // jadeite::Timer
