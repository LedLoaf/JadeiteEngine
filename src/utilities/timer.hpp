#pragma once 
#include <chrono>

namespace jadeite
{
	namespace utilities
	{
		/* A timer class to start, stop, pause, resume, and restart a timer and return it in elapsed milliseconds and seconds */
		class Timer
		{
		public:
			Timer() = default;
			~Timer() = default;
			
			void Start();
			void Stop();
			void Pause();
			void Resume();
			void Restart();
			
			int64_t ElapsedMS() const;
			int64_t ElapsedSec() const;
			
			inline bool IsRunning() const { return m_bIsRunning; }
			inline bool IsPaused() const { return m_bIsPaused; }
			
		private:
			std::chrono::time_point<std::chrono::steady_clock> m_StartPoint;
			std::chrono::time_point<std::chrono::steady_clock> m_PausedPoint;
			bool m_bIsRunning{ false };
			bool m_bIsPaused{ false };
		};	
	} // utilities
} // jadeite