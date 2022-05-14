/*********************************************************************
 *  LogSystem.h
 *  Copyright (C) 2020 Jayou. All Rights Reserved.
 * 
 *  A set of logging utility API.
 *********************************************************************/

#pragma once

#include "../Color/ColorManager.h"
#include "../Timer/TimerManager.h"
#include "../String/StringManager.h"

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
		TimerUtil::TimeStamp TimeStamp;          ///< The time stamp when generating this log.
		LogSystem::Category  Category;           ///< The category of this log.
		LogSystem::Level     Level;              ///< The level of this log.
		ColorUtil::Color     Color;              ///< The color of this log when displaying.
		string               Content;            ///< The content of this log.
		string               CustomCategory;     ///< The custom category of this log.

		LogInfo(
			const TimerUtil::TimeStamp& InTimeStamp, 
			const LogSystem::Category& InCategory, 
			const LogSystem::Level& InLevel, 
			const ColorUtil::Color& InColor, 
			const string& InContent, 
			const string& InCustomCategory);

		/**
		 *  Convert level enum to string.
		 * 
		 *  @return the string represent the level.
		 */
		string LevelToString() const;

		/**
		 *  Convert category enum to string.
		 * 
		 *  @return the string represent the category.
		 */
		string CategoryToString() const;

		/**
		 *  Resolve a log string from LogInfo.
		 * 
		 *  @return the log string to output.
		 */
		string ToString() const;
	};

public:

	/**
	 *  Outputs a specific level message to log with a given logging category.
	 * 
	 *  @param  InLog       the log to output.
	 *  @param  InCategory  the catagory this log belong to.
	 *  @param  InLevel     the level this log belong to.
	 *  @param  InColor     the color of this log when displaying.
	 */
	static void Log(const string& InLog, const Category& InCategory = Category::Unknown, const Level& InLevel = Level::Common, const ColorUtil::Color& InColor = ColorUtil::Color::Snow);

	/**
	 *  Outputs a specific level message to log with a given custom logging category.
	 * 
	 *  @param  InLog       the log to output.
	 *  @param  InCategory  the custom catagory this log belong to.
	 *  @param  InLevel     the level this log belong to.
	 *  @param  InColor     the color of this log when displaying.
	 */
	static void Log(const string& InLog, const string& InCategory, const Level& InLevel = Level::Common, const ColorUtil::Color& InColor = ColorUtil::Color::Snow);

	/**
	 *  Outputs a warning message to log with a given logging category.
	 * 
	 *  @param  InLog       the log to output.
	 *  @param  InCategory  the catagory this log belong to.
	 */
	static void LogWarning(const string& InLog, const Category& InCategory = Category::Unknown);

	/**
	 *  Outputs a warning message to log with a given custom logging category.
	 * 
	 *  @param  InLog       the log to output.
	 *  @param  InCategory  the custom catagory this log belong to.
	 */
	static void LogWarning(const string& InLog, const string& InCategory);

	/**
	 *  Outputs a error message to log with a given logging category.
	 * 
	 *  @param  InLog       the log to output.
	 *  @param  InCategory  the catagory this log belong to.
	 */
	static void LogError(const string& InLog, const Category& InCategory = Category::Unknown);

	/**
	 *  Outputs a error message to log with a given custom logging category.
	 * 
	 *  @param  InLog       the log to output.
	 *  @param  InCategory  the custom catagory this log belong to.
	 */
	static void LogError(const string& InLog, const string& InCategory);
};

#define ENABLE_LOG_SYSTEM
#ifdef  ENABLE_LOG_SYSTEM

/**
 *  A macro that outputs a common message to log with a given logging category.
 * 
 *  @param  log       the log string to output.
 *  @param  category  the category this log belong to.
 */
#define _log_common(log, category)  LogSystem::Log(log, category);

/**
 *  A macro that outputs a warning message to log with a given logging category.
 * 
 *  @param  log       the log string to output.
 *  @param  category  the category this log belong to.
 */
#define _log_warning(log, category) LogSystem::LogWarning(StringUtil::Printf("% (Detail at file: %, line %)", log, __FILE__, __LINE__), category);

/**
 *  A macro that outputs a error message to log with a given logging category.
 * 
 *  @param  log       the log string to output.
 *  @param  category  the category this log belong to.
 */
#define _log_error(log, category)   LogSystem::LogError(StringUtil::Printf("% (Detail at file: %, line %)", log, __FILE__, __LINE__), category);

#else

#define _log_common(log, category)
#define _log_warning(log, category)
#define _log_error(log, category)

#endif // ENABLE_LOG_SYSTEM
