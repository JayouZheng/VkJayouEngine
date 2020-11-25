//
// LogSystem.h
//

#pragma once

#include "Core/Utility/Color/ColorManager.h"
#include "Core/Utility/Timer/TimerManager.h"
#include "Core/Utility/String/StringManager.h"

class LogSystem
{

public:

	enum class Level
	{
		Common = 0,
		Warning,
		Error,

		Max = 0xff
	};

	enum class Category
	{
		Unknown = 0,

		VulkanAPI,

		Global,
		BaseLayer,
		LogicalDevice,
		CommandList,
		RenderPass,
		GLSLCompiler, 
		JsonParser, 
		VkSmartPtr,
		ModuleLoader,
		PathParser,
		PerformanceScope,

		IO,
		Memory,


		// Keep it for future use.
		Engine,
		Editor,
		Game,

		// User Custom.
		Custom,

		Max = 0xff
	};

	struct LogInfo
	{	
		TimerUtil::TimeStamp TimeStamp;
		LogSystem::Category  Category;
		LogSystem::Level     Level;
		ColorUtil::Color     Color;
		string          Content;
		string          CustomCategory;

		string LevelToString() const;
		string CategoryToString() const;

		string ToString() const;
	};

public:

	static void Log        (const string& InLog, const Category& InCategory = Category::Unknown, const Level& InLevel = Level::Common, const ColorUtil::Color& InColor = ColorUtil::Color::Snow);
	static void Log        (const string& InLog, const string& InCategory, const Level& InLevel = Level::Common, const ColorUtil::Color& InColor = ColorUtil::Color::Snow);
	static void LogError   (const string& InLog, const Category& InCategory = Category::Unknown);
	static void LogError   (const string& InLog, const string& InCategory);
	static void LogWarning (const string& InLog, const Category& InCategory = Category::Unknown);
	static void LogWarning (const string& InLog, const string& InCategory);
};

#define ENABLE_LOG_SYSTEM
#ifdef  ENABLE_LOG_SYSTEM

#define _log_common(log, category)  LogSystem::Log(log, category);
#define _log_warning(log, category) LogSystem::LogWarning(StringUtil::Printf("% (Detail at file: %, line %)", log, __FILE__, __LINE__), category);
#define _log_error(log, category)   LogSystem::LogError(StringUtil::Printf("% (Detail at file: %, line %)", log, __FILE__, __LINE__), category);

#endif // ENABLE_LOG_SYSTEM
