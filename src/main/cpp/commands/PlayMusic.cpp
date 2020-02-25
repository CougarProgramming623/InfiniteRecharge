#include "commands/PlayMusic.h"
#include "Robot.h"

#include "ohs/Formatter.h"
#include "ohs/Log.h"

#include <wpi/ArrayRef.h>
#include <frc2/command/PrintCommand.h>
#include <frc/DriverStation.h>
#include "Util.h"

#include "math.h"

namespace ohs623 {

	PlayMusic::PlayMusic() {
		AddRequirements({ &ohs2020::Robot::Get().GetDriveTrain() });
	}

    void PlayMusic::Initialize() {
		DebugOutF("Playing");
		Music::GetOrchestra().Play();
	}
	
	void PlayMusic::Execute() {}

	bool PlayMusic::IsFinished() {
		frc::Joystick& stick = ohs2020::Robot::Get().GetOI().GetDriverJoystick();
		const float deadBand = 0.2f;
		if (abs(stick.GetRawAxis(0)) > deadBand || abs(stick.GetRawAxis(1)) > deadBand || abs(stick.GetRawAxis(2)) > deadBand) {
			return true;
		}

		return false;
	}

	void PlayMusic::End(bool interrupted) {

	}


}//namespace