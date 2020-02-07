#pragma once

#include "Formatter.h"

#include <frc/DriverStation.h>

namespace ohs623 {

	enum class LogLevel {
		TRACE, DEBUG, INFO, WARN, ERROR, OFF
	};

	//Returns the current log level
	LogLevel GetCurrentLevel();

	template<std::size_t Len>
	void LogMessage(SizedFormatter<Len> formatter, LogLevel level, const char* file = "Unknown", int line = -1) {
		//Dont print messages that are below the given log level
		if (level < GetCurrentLevel()) { return; }

		static SizedFormatter<Len + 64> finalFormatter;
		finalFormatter << formatter;
		//Insert location information is it was provided
		if (line != -1 /* && level >= LogLevel::WARN */ ) {
			finalFormatter << " (In \"" << file << "\" At line: " << line << ')';
		}

		finalFormatter << '\n';

		//Log the formatted result to different sources depending on the log level
		if (level < LogLevel::WARN) 		frc2::CommandScheduler::GetInstance().Schedule(new frc2::PrintCommand(finalFormatter.c_str()));
		else if (level <= LogLevel::WARN)	frc::DriverStation::ReportWarning(finalFormatter.c_str());
		else 								frc::DriverStation::ReportError(finalFormatter.c_str());

	}

	template<typename F>
	void Log(LogLevel level, F func, const char* file = "Unknown", int line = -1) {
		DefaultFormatter formatter;
		func(formatter);
		LogMessage(formatter, level, file, line);
	}

}

//A newline is automatically inserted after each call to OHS_XX
#define OHS_TRACE(func)	ohs623::Log(ohs623::LogLevel::TRACE, func, __FILE__, __LINE__);
#define OHS_DEBUG(func)	ohs623::Log(ohs623::LogLevel::DEBUG, func, __FILE__, __LINE__);
#define OHS_INFO(func)	ohs623::Log(ohs623::LogLevel::INFO,  func, __FILE__, __LINE__);
#define OHS_WARN(func)	ohs623::Log(ohs623::LogLevel::WARN,  func, __FILE__, __LINE__);
#define OHS_ERROR(func)	ohs623::Log(ohs623::LogLevel::ERROR, func, __FILE__, __LINE__);


