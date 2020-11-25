//
// LogSystem.cpp
//

#include "LogSystem.h"

namespace
{
	static std::vector<LogSystem::LogInfo> g_logs;

	void InternalLog(
		const string&         InLog,
		const LogSystem::Category& InCategory,
		const string&         InCustomCategory,
		const LogSystem::Level&    InLevel, 
		const ColorUtil::Color&    InColor)
	{
		ColorUtil::Color color = InColor;

		switch (InLevel)
		{
		case LogSystem::Level::Error:
			color = ColorUtil::Color::Red;
			break;
		case LogSystem::Level::Warning:
			color = ColorUtil::Color::Yellow;
			break;
		default:
			break;
		}

		LogSystem::LogInfo logInfo = { TimerUtil::GetCurrentTimeStamp(), InCategory, InLevel, color, InLog, InCustomCategory };

		// TODO: Output log.
		_cmd_print_line(logInfo.ToString());

		g_logs.push_back(logInfo);
	}
}

void LogSystem::Log(const string& InLog, const Category& InCategory, const Level& InLevel, const ColorUtil::Color& InColor)
{
	InternalLog(InLog, InCategory, _str_null, InLevel, InColor);
}

void LogSystem::Log(const string& InLog, const string& InCategory, const Level& InLevel, const ColorUtil::Color& InColor)
{
	InternalLog(InLog, Category::Custom, InCategory, InLevel, InColor);
}

void LogSystem::LogError(const string& InLog, const Category& InCategory)
{
	Log(InLog, InCategory, Level::Error);
}

void LogSystem::LogError(const string& InLog, const string& InCategory)
{
	Log(InLog, InCategory, Level::Error);
}

void LogSystem::LogWarning(const string& InLog, const Category& InCategory)
{
	Log(InLog, InCategory, Level::Warning);
}

void LogSystem::LogWarning(const string& InLog, const string& InCategory)
{
	Log(InLog, InCategory, Level::Warning);
}

string LogSystem::LogInfo::LevelToString() const
{
#define LOG_LEVEL_TO_STRING(level) if (Level == LogSystem::Level::level) return _name_of(level);

	//LOG_LEVEL_TO_STRING(Common);
	if (Level == LogSystem::Level::Common) return "";

	LOG_LEVEL_TO_STRING(Warning);
	LOG_LEVEL_TO_STRING(Error);

	return _name_of(Common);
}

string LogSystem::LogInfo::CategoryToString() const
{
#define LOG_CATEGORY_TO_STRING(category) if (Category == LogSystem::Category::category) return _name_of(category);

	LOG_CATEGORY_TO_STRING(Unknown);

	LOG_CATEGORY_TO_STRING(VulkanAPI);

	LOG_CATEGORY_TO_STRING(Global);
	LOG_CATEGORY_TO_STRING(BaseLayer);
	LOG_CATEGORY_TO_STRING(LogicalDevice);
	LOG_CATEGORY_TO_STRING(CommandList);
	LOG_CATEGORY_TO_STRING(RenderPass);
	LOG_CATEGORY_TO_STRING(GLSLCompiler);
	LOG_CATEGORY_TO_STRING(JsonParser);
	LOG_CATEGORY_TO_STRING(VkSmartPtr);
	LOG_CATEGORY_TO_STRING(ModuleLoader);
	LOG_CATEGORY_TO_STRING(PathParser);
	LOG_CATEGORY_TO_STRING(PerformanceScope);

	LOG_CATEGORY_TO_STRING(IO);
	LOG_CATEGORY_TO_STRING(Memory);

	// Keep it for future use.
	LOG_CATEGORY_TO_STRING(Engine);
	LOG_CATEGORY_TO_STRING(Editor);
	LOG_CATEGORY_TO_STRING(Game);

	// User Custom.
	if (Category == LogSystem::Category::Custom)
		return CustomCategory;

	return _str_unknown;
}

string LogSystem::LogInfo::ToString() const
{
	return Level != LogSystem::Level::Common ? StringUtil::Printf("[%][%] !!!%: %", TimeStamp.ToString(), CategoryToString(), LevelToString(), Content) : StringUtil::Printf("[%][%]>%", TimeStamp.ToString(), CategoryToString(), Content);
}
