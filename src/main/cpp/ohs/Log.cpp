#include "ohs/Log.h"

#include "ohs/Formatter.h"

#include <frc2/command/PrintCommand.h>
#include <frc2/command/CommandScheduler.h>


namespace ohs623 {

	void LogMessage(const char* message, LogLevel level, const char* file, int line) {
		//Dont print messages that are below the given log level
		if (level < GetCurrentLevel()) { return; }

		//Insert location information is it was provided
		if (line != -1 /* && level >= LogLevel::WARN */ ) {
			static SizedFormatter<512> formatter;
			formatter << message << "\n"/*" (In \"" << file << "\" line: " << line << ")"*/;
			message = formatter.c_str();
		}

		//Log the message to different sources depending on the log level
		if (level < LogLevel::WARN) 		frc2::CommandScheduler::GetInstance().Schedule(new frc2::PrintCommand(message));
		else if (level <= LogLevel::WARN)	frc::DriverStation::ReportWarning(message);
		else 								frc::DriverStation::ReportError(message);

	}

	LogLevel GetCurrentLevel() {
		return LogLevel::TRACE;//TODO: only show warnings/errors when in competitions
	}
}
