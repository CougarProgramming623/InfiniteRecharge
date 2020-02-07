#include "ohs/Log.h"

#include "ohs/Formatter.h"

#include <frc2/command/PrintCommand.h>
#include <frc2/command/CommandScheduler.h>


namespace ohs623 {

	LogLevel GetCurrentLevel() {
		return LogLevel::TRACE;//TODO: only show warnings/errors when in competitions
	}
}
