//
// LogSystem.h
//

#pragma once

#include "Global.h"
#include "Color.h"

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

	struct LogInfo
	{
		LogSystem::Level Level;
		std::string      Content;
	};

public:

	LogSystem() {}
	~LogSystem() {}

	void Log(const std::string& InLog, const Level& InLevel = Level::Common, const Color& InColor = Color::Snow) const;

protected:

	
};