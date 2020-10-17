//
// LogSystem.cpp
//

#include "LogSystem.h"
#include "StringManager.h"

namespace
{
	static std::vector<LogSystem::LogInfo> g_logs;
}

void LogSystem::Log(
	const std::string& InLog, 
	const Category&    InCategory, 
	const Level&       InLevel, 
	const Color&       InColor)
{
	LogSystem::LogInfo logInfo = { TimerUtil::GetCurrentTimeStamp(), InCategory, InLevel, InColor, InLog };

	// TODO: Output log.
	_cmd_print_line(logInfo.ToString());

	g_logs.push_back(logInfo);
}

std::string LogSystem::LogInfo::LevelToString() const
{
#define LOG_LEVEL_TO_STRING(level) if (Level == LogSystem::Level::level) return _name_of(level);

	LOG_LEVEL_TO_STRING(Common);
	LOG_LEVEL_TO_STRING(Warning);
	LOG_LEVEL_TO_STRING(Error);

	return _name_of(Common);
}

std::string LogSystem::LogInfo::CategoryToString() const
{
#define LOG_CATEGORY_TO_STRING(category) if (Category == LogSystem::Category::category) return _name_of(category);

	LOG_CATEGORY_TO_STRING(Unknown);
	LOG_CATEGORY_TO_STRING(BaseLayer);
	LOG_CATEGORY_TO_STRING(LogicalDevice);
	LOG_CATEGORY_TO_STRING(CommandList);
	LOG_CATEGORY_TO_STRING(GLSLCompiler);
	LOG_CATEGORY_TO_STRING(JsonParser);
	LOG_CATEGORY_TO_STRING(VkSmartPtr);
	LOG_CATEGORY_TO_STRING(DiskResourceLoader);

	// Keep it for future use.
	LOG_CATEGORY_TO_STRING(Engine);
	LOG_CATEGORY_TO_STRING(Editor);
	LOG_CATEGORY_TO_STRING(Game);

	return _str_unknown;
}

std::string LogSystem::LogInfo::ToString() const
{
	return StringUtil::Printf("[%][%] %: %", TimeStamp.ToString(), CategoryToString(), LevelToString(), Content);
}
