//
// LogSystem.h
//

#pragma once

#include "Color.h"
#include "TimerManager.h"

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

		BaseLayer,
		LogicalDevice,
		CommandList,
		GLSLCompiler, 
		JsonParser, 
		VkSmartPtr,
		DiskResourceLoader,


		// Keep it for future use.
		Engine,
		Editor,
		Game,

		Max = 0xff
	};

	struct LogInfo
	{	
		TimerUtil::TimeStamp TimeStamp;
		LogSystem::Category  Category;
		LogSystem::Level     Level;
		ColorUtil::Color     Color;
		std::string          Content;

		std::string LevelToString() const;
		std::string CategoryToString() const;

		std::string ToString() const;
	};

public:

	static void Log(const std::string& InLog, const Category& InCategory = Category::Unknown, const Level& InLevel = Level::Common, const Color& InColor = Color::Snow);
};