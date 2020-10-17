//
// TimerManager.h
//

#pragma once

#include "TypeDef.h"

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
};