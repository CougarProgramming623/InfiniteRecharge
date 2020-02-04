#include "ohs/Log.h"

#include "ohs/Formatter.h"

#include <frc/DriverStation.h>
#include <frc2/command/PrintCommand.h>
#include <frc2/command/CommandScheduler.h>


namespace ohs623 {

	void LogMessage(const char* message, LogLevel level, const char* file, int line) {
		//Dont print messages that are below the given log level
		if (level < GetCurrentLevel()) { frc::DriverStation::ReportWarning("discarding"); return; }
		frc::DriverStation::ReportWarning("Logging...");

		//Insert location information is it was provided
		if (line != -1) {
			static SizedFormatter<512> formatter;
			formatter << message << " (In \"" << file << "\" line: " << line << ")";
			message = formatter.c_str();
		}

		//Log the message to different sources depending on the log level
		if (level < LogLevel::WARN) 		frc2::CommandScheduler::GetInstance().Schedule(new frc2::PrintCommand(message));
		else if (level <= LogLevel::WARN) 	frc::DriverStation::ReportWarning(message);
		else 								frc::DriverStation::ReportError(message);

	}

	LogLevel GetCurrentLevel() {
		return LogLevel::TRACE;//TODO: only show warnings/errors when in competitions
	}
}
