//
// LogSystem.h
//

#pragma once

#include "Color.h"
#include "TimerManager.h"
#include "StringManager.h"

using namespace ColorUtil;

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
		std::string          Content;
		std::string          CustomCategory;

		std::string LevelToString() const;
		std::string CategoryToString() const;

		std::string ToString() const;
	};

public:

	static void Log        (const std::string& InLog, const Category& InCategory = Category::Unknown, const Level& InLevel = Level::Common, const Color& InColor = Color::Snow);
	static void Log        (const std::string& InLog, const std::string& InCategory, const Level& InLevel = Level::Common, const Color& InColor = Color::Snow);
	static void LogError   (const std::string& InLog, const Category& InCategory = Category::Unknown);
	static void LogError   (const std::string& InLog, const std::string& InCategory);
	static void LogWarning (const std::string& InLog, const Category& InCategory = Category::Unknown);
	static void LogWarning (const std::string& InLog, const std::string& InCategory);
};

#define _log_warning(log, category) LogSystem::LogWarning(StringUtil::Printf("% (Detail at file: %, line %)", log, __FILE__, __LINE__), category);
#define _log_error(log, category)   LogSystem::LogError(StringUtil::Printf("% (Detail at file: %, line %)", log, __FILE__, __LINE__), category);
