//
// TimerManager.h
//

#pragma once

#include "Core/TypeDef.h"
#include <chrono>

using hclock = std::chrono::high_resolution_clock;
using htime_point = hclock::time_point;

class TimerUtil
{
	
public:

	struct TimeStamp
	{
		uint16 DayOfYear;
		uint16 Year;

		uint8  Month;
		uint8  Day;

		uint8  Hour;
		uint8  Minute;
		uint8  Second;

		uint8  Week;

		string ToString() const;

		string GetWeek() const;
		string GetWeek_Ch() const;

		string GetMonth() const;
		string GetMonth_Ch() const;
	};

	static TimeStamp GetCurrentTimeStamp();

	class PerformanceCounter
	{
	public:

		void   BeginCounter      (const string& InName);
		void   EndCounter        (const string& InName);
		double GetCounterResult  (const string& InName);

	private:

		std::unordered_map<string, htime_point> m_counterBeginPoint;
		std::unordered_map<string, double>      m_counterResults;
	};

	class PerformanceScope
	{

	public:

		PerformanceScope(const string& InScopeName);
		~PerformanceScope();

	private:

		string      m_scopeName;
		htime_point m_beginPoint;
	};
};