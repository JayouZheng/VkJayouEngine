//
// TimerManager.cpp
//

#include "TimerManager.h"
#include "StringManager.h"
#include "LogSystem.h"
#include <ctime>

namespace
{
    std::array<std::string, 7> EnWeekName =
    {
        "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"
    };

    std::array<std::string, 7> ChWeekName =
    {
        _u8_text("星期天"), _u8_text("星期一"), _u8_text("星期二"), _u8_text("星期三"), _u8_text("星期四"), _u8_text("星期五"), _u8_text("星期六")
    };

    std::array<std::string, 12> EnMonthName =
    {
        "January", "February", "March",     "April",   "May",      "June",
        "July",    "August",   "September", "October", "November", "December"
    };

    std::array<std::string, 12> ChMonthName =
    {
        _u8_text("一月"), _u8_text("二月"), _u8_text("三月"), _u8_text("四月"), _u8_text("五月"),   _u8_text("六月"),
        _u8_text("七月"), _u8_text("八月"), _u8_text("九月"), _u8_text("十月"), _u8_text("十一月"), _u8_text("十二月")
    };
}

TimerUtil::TimeStamp TimerUtil::GetCurrentTimeStamp()
{
    std::time_t result = std::time(nullptr);

    std::tm timeinfo;
    localtime_s(&timeinfo, &result);

    TimerUtil::TimeStamp timestamp;
    timestamp.DayOfYear = (uint16)timeinfo.tm_yday;
    timestamp.Year      = (uint16)timeinfo.tm_year + 1900;
    timestamp.Month     = (uint8) timeinfo.tm_mon  + 1;
    timestamp.Day       = (uint8) timeinfo.tm_mday;
    timestamp.Hour      = (uint8) timeinfo.tm_hour;
    timestamp.Minute    = (uint8) timeinfo.tm_min;
    timestamp.Second    = (uint8) timeinfo.tm_sec;
    timestamp.Week      = (uint8) timeinfo.tm_wday;

    return timestamp;
}

std::string TimerUtil::TimeStamp::ToString() const
{
    // If Input is uint8, It Will Be Treated as char!!!
    return StringUtil::Printf("%.%.%-%:%:%", (uint32)Year, (uint32)Month, (uint32)Day, (uint32)Hour, (uint32)Minute, (uint32)Second);
}

std::string TimerUtil::TimeStamp::GetWeek() const
{
    return EnWeekName[Week];
}

std::string TimerUtil::TimeStamp::GetWeek_Ch() const
{
    return ChWeekName[Week];
}

std::string TimerUtil::TimeStamp::GetMonth() const
{
    return EnMonthName[Month - 1];
}

std::string TimerUtil::TimeStamp::GetMonth_Ch() const
{
    return ChMonthName[Month - 1];
}

void TimerUtil::PerformanceCounter::BeginCounter(const std::string& InName)
{
    if (m_counterResults.find(InName) != m_counterResults.end())
        m_counterResults.erase(InName);
    htime_point beginPoint = hclock::now();
    m_counterBeginPoint[InName] = beginPoint;
}

void TimerUtil::PerformanceCounter::EndCounter(const std::string& InName)
{
    if (m_counterResults.find(InName) != m_counterResults.end())
        return;
    htime_point endPoint = hclock::now();
    m_counterResults[InName] = std::chrono::duration_cast<std::chrono::duration<double>>(endPoint - m_counterBeginPoint[InName]).count();
}

double TimerUtil::PerformanceCounter::GetCounterResult(const std::string& InName)
{
    if (m_counterResults.find(InName) != m_counterResults.end())
        return m_counterResults[InName];
    else return 0.0f;
}

TimerUtil::PerformanceScope::PerformanceScope(const std::string& InScopeName)
{
    m_scopeName  = InScopeName;
    m_beginPoint = hclock::now();
}

TimerUtil::PerformanceScope::~PerformanceScope()
{
    htime_point endPoint = hclock::now();
    double result = std::chrono::duration_cast<std::chrono::duration<double>>(endPoint - m_beginPoint).count();
    LogSystem::Log(StringUtil::Printf("Scope Name: %, Elapsed Time: %s", m_scopeName, result), LogSystem::Category::PerformanceScope);
}
