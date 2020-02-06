#pragma once

#include "Formatter.h"

#include <frc/DriverStation.h>

namespace ohs623 {

	enum class LogLevel {
		TRACE, DEBUG, INFO, WARN, ERROR, OFF
	};

	//Returns the current log level
	LogLevel GetCurrentLevel();

	void LogMessage(const char* message, LogLevel level, const char* file = "Unknown", int line = -1);

	template<typename F>
	void Log(LogLevel level, F func, const char* file = "Unknown", int line = -1) {
		DefaultFormatter formatter;
		func(formatter);
		LogMessage(formatter.c_str(), level, file, line);
	}

}


#define OHS_TRACE(func)	ohs623::Log(ohs623::LogLevel::TRACE, func, __FILE__, __LINE__);
#define OHS_DEBUG(func)	ohs623::Log(ohs623::LogLevel::DEBUG, func, __FILE__, __LINE__);
#define OHS_INFO(func)	ohs623::Log(ohs623::LogLevel::INFO,  func, __FILE__, __LINE__);
#define OHS_WARN(func)	ohs623::Log(ohs623::LogLevel::WARN,  func, __FILE__, __LINE__);
#define OHS_ERROR(func)	ohs623::Log(ohs623::LogLevel::ERROR, func, __FILE__, __LINE__);


