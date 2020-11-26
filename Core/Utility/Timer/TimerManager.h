/*********************************************************************
 *  TimerManager.h
 *  Copyright (C) 2020 Jayou. All Rights Reserved.
 * 
 *  A set of timing utility API..
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
};