//
// LogSystem.h
//

#pragma once

#include "Global.h"

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

	using LogLevel = Level;

	struct LogInfo
	{
		LogLevel Level;
		std::string Content;
	};

public:

	LogSystem() {}
	~LogSystem() {}

	void Log(const std::string& InLog, const LogLevel& InLevel = LogLevel::Common) const;

protected:

	
};