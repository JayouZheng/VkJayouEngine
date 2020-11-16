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

		std::string ToString() const;

		std::string GetWeek() const;
		std::string GetWeek_Ch() const;

		std::string GetMonth() const;
		std::string GetMonth_Ch() const;
	};

	static TimeStamp GetCurrentTimeStamp();

	class PerformanceCounter
	{
	public:

		void   BeginCounter      (const std::string& InName);
		void   EndCounter        (const std::string& InName);
		double GetCounterResult  (const std::string& InName);

	private:

		std::unordered_map<std::string, htime_point> m_counterBeginPoint;
		std::unordered_map<std::string, double> m_counterResults;
	};

	class PerformanceScope
	{

	public:

		PerformanceScope(const std::string& InScopeName);
		~PerformanceScope();

	private:

		std::string m_scopeName;
		htime_point m_beginPoint;
	};
};