/*********************************************************************
 *  TimerManager.h
 *  Copyright (C) 2020 Jayou. All Rights Reserved.
 * 
 *  A set of timing utility API.
 *********************************************************************/

#pragma once

#include "Core/TypeDef.h"
#include <chrono>

using hclock = std::chrono::high_resolution_clock;
using htime_point = hclock::time_point;

class TimerUtil
{
	
public:

	//---------------------------------------------------------------------------
	// Helper structure for storing a time stamp.
	//---------------------------------------------------------------------------

	struct TimeStamp
	{
		uint16 DayOfYear;      ///< Days since January 1 - [0, 365].
		uint16 Year;           ///< Years since 1900.

		uint8  Month;          ///< Months since January - [0, 11].
		uint8  Day;            ///< Day of the month - [1, 31].

		uint8  Hour;           ///< Hours since midnight - [0, 23].
		uint8  Minute;         ///< Minutes after the hour - [0, 59].
		uint8  Second;         ///< Seconds after the minute - [0, 60] including leap second.

		uint8  Week;           ///< Days since Sunday - [0, 6].

		/**
		 *  Convert TimeStamp to a human readable string.
		 * 
		 *  @return a string represent a time stamp.
		 */
		string ToString() const;

		/**
		 *  Resolve week from time stamp.
		 * 
		 *  @return a string represent week in English.
		 */
		string GetWeek() const;

		/**
		 *  Resolve week from time stamp.
		 * 
		 *  @return a string represent week in Chinese.
		 */
		string GetWeek_Ch() const;

		/**
		 *  Resolve month from time stamp.
		 * 
		 *  @return a string represent month in English.
		 */
		string GetMonth() const;

		/**
		 *  Resolve month from time stamp.
		 * 
		 *  @return a string represent week in Chinese.
		 */
		string GetMonth_Ch() const;
	};

	/**
	 *  Resolve current system local time stamp.
	 * 
	 *  @return local time stamp.
	 */
	static TimeStamp GetCurrentTimeStamp();

	//---------------------------------------------------------------------------
	// Helper class for counting how much time has been taken to run some functions.
	//---------------------------------------------------------------------------

	class PerformanceCounter
	{
	public:

		/**
		 *  Lauch/Begin a counter with specific name.
		 * 
		 *  @param  InName  the counter name.
		 */
		void BeginCounter(const string& InName);

		/**
		 *  Finish/End a counter by the specific name.
		 * 
		 *  @param  InName  the counter name.
		 */
		void EndCounter(const string& InName);

		/**
		 *  Resolve the counter result by the specific name.
		 * 
		 *  @param  InName  the counter name.
		 * 
		 *  @return the counter result in millisecond.
		 */
		double GetCounterResult(const string& InName);

	private:

		std::unordered_map<string, htime_point> m_counterBeginPoint;   ///< Caching each counter's initial time stamp.
		std::unordered_map<string, double>      m_counterResults;      ///< Caching each counter's final result in ms.
	};

	//---------------------------------------------------------------------------
	// Helper class for counting how much time has been taken to run code at specific scope.
	//---------------------------------------------------------------------------

	class PerformanceScope
	{

	public:

		/**
		 *  Construct an internal counter within the scope.
		 * 
		 *  @param  InScopeName  current scopename for logging usage.
		 */
		PerformanceScope(const string& InScopeName);

		~PerformanceScope();

	private:

		string      m_scopeName;       ///< Cache scope name for logging usage. 
		htime_point m_beginPoint;      ///< Cache internal counter's initial time stamp.
	};

	//---------------------------------------------------------------------------
	// Helper class for animation and simulation timing.
	//---------------------------------------------------------------------------

	class GameTimer
	{
	public:

		GameTimer() noexcept(false) :
			m_deltaTime(0),
			m_totalTime(0),
			m_leftOverTime(0),
			m_frameCount(0),
			m_framesPerSecond(0),
			m_framesThisSecond(0),
			m_oneSecond(0),
			m_isFixedTimeStep(false),
			m_targetDelta(1.0f / 60) // 60 frames per second.
		{
			m_lastTime = hclock::now();

			// Initialize max delta to 1/10 of a second (10 frames per second).
			m_maxDelta = 0.1f;
		}

		// Get delta time since the previous Update call.
		double GetDeltaSeconds() const { return m_deltaTime; }

		// Get total time since the start of the program.
		double GetTotalSeconds() const { return m_totalTime; }

		// Get total number of updates since start of the program.
		uint32 GetFrameCount() const { return m_frameCount; }

		// Get the current framerate.
		uint32 GetFramesPerSecond() const { return m_framesPerSecond; }

		// Set whether to use fixed or variable timestep mode.
		void SetFixedTimeStep(bool isFixedTimestep) { m_isFixedTimeStep = isFixedTimestep; }

		// Set how often to call Update when in fixed timestep mode.
		void SetTargetDeltaSeconds(double targetDelta) { m_targetDelta = targetDelta; }

		// After an intentional timing discontinuity (for instance a blocking IO operation)
		// call this to avoid having the fixed timestep logic attempt a set of catch-up 
		// Update calls.

		void Reset()
		{
			m_lastTime = hclock::now();

			m_oneSecond = 0.0f;
			m_leftOverTime = 0.0f;
			m_framesPerSecond = 0;
			m_framesThisSecond = 0;
		}

		// Update timer state, calling the specified Update function the appropriate number of times.
		template<typename TUpdate>
		void Tick(const TUpdate& update)
		{
			// Query the current time.
			htime_point currentTime = hclock::now();

			double deltaTime = std::chrono::duration_cast<std::chrono::duration<double>>(currentTime - m_lastTime).count();

			m_lastTime = currentTime;
			m_oneSecond += deltaTime;

			// Clamp excessively large time deltas (e.g. after paused in the debugger).
			if (deltaTime > m_maxDelta)
			{
				deltaTime = m_maxDelta;
			}

			uint32 lastFrameCount = m_frameCount;

			if (m_isFixedTimeStep)
			{
				// Fixed timestep update logic

				// If the app is running very close to the target elapsed time (within 1/4 of a millisecond) just clamp
				// the clock to exactly match the target value. This prevents tiny and irrelevant errors
				// from accumulating over time. Without this clamping, a game that requested a 60 fps
				// fixed update, running with vsync enabled on a 59.94 NTSC display, would eventually
				// accumulate enough tiny errors that it would drop a frame. It is better to just round 
				// small deviations down to zero to leave things running smoothly.
				// For example, 49 +49 -50 = 48, 48 +49 -50 = 47, 47 +49 -50 = 46...drop first frame.

				if (std::abs(deltaTime - m_targetDelta) < 0.000025f)
				{
					deltaTime = m_targetDelta; // Clamp to Target ElapsedT icks.
				}

				m_leftOverTime += deltaTime;

				while (m_leftOverTime >= m_targetDelta)
				{
					m_deltaTime = m_targetDelta;
					m_totalTime += m_targetDelta;
					m_leftOverTime -= m_targetDelta;
					m_frameCount++;

					update();
				}
			}
			else
			{
				// Variable timestep update logic.
				m_deltaTime = deltaTime;
				m_totalTime += deltaTime;
				m_leftOverTime = 0;
				m_frameCount++;

				update();
			}

			// Track the current framerate.
			if (m_frameCount != lastFrameCount)
			{
				m_framesThisSecond++;
			}

			if (m_oneSecond >= 1.0f)
			{
				m_framesPerSecond = m_framesThisSecond;
				m_framesThisSecond = 0;
				m_oneSecond = std::fmod(m_oneSecond, 1.0f);
			}
		}

	private:

		// Source timing data in second.
		htime_point m_lastTime;
		double m_maxDelta;
		double m_deltaTime;
		double m_totalTime;
		double m_leftOverTime;

		// Members for tracking the framerate.
		uint32 m_frameCount;
		uint32 m_framesPerSecond;
		uint32 m_framesThisSecond;
		double m_oneSecond;

		// Members for configuring fixed timestep mode.
		bool   m_isFixedTimeStep;
		double m_targetDelta;
	};
};