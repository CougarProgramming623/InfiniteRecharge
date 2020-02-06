
#include <frc/DriverStation.h>

#include <stdlib.h>

#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)

bool CanAssertionsQuit();

//Use OHS_ASSERT ensure things are operating normally during the robot's executition
//The condition is a boolean expression that is expected to be true
//An assertion failing is expected to be an unrecoverable event.
//Message is printed to the console along with debugging information if the assertion is false.
//If CanAssertionsQuit() is true and an assertion fails, the program will exit
//Changing CanAssertionsQuit() to return false will change this behavior to keep running the program
//when an assertion fails.

#define OHS_ASSERT(condition, message)																		\
	if (!(condition)) {																						\
		frc::DriverStation::ReportError("ASSERTION FAILED");												\
		frc::DriverStation::ReportError(message);															\
		frc::DriverStation::ReportError("At File: \"" __FILE__ "\" Line " TOSTRING(__LINE__));				\
																											\
		if (CanAssertionsQuit()) {																			\
			frc::DriverStation::ReportError("Program will terminate");										\
			exit(EXIT_FAILURE);																				\
		} else																								\
			frc::DriverStation::ReportError("FIX THIS.");													\
	}																										\


