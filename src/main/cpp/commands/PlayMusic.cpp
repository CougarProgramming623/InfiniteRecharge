#include "commands/PlayMusic.h"
#include "Robot.h"

#include "ohs/Formatter.h"
#include "ohs/Log.h"

#include <wpi/ArrayRef.h>
#include <frc2/command/PrintCommand.h>
#include <frc/DriverStation.h>
#include "Util.h"
#include "ohs/Music.h"

#include "math.h"

namespace ohs2020 {

	PlayMusic::PlayMusic() {
		AddRequirements({ &Robot::Get().GetDriveTrain() });
	}

    void PlayMusic::Initialize() {
		Music::Start();
		OHS_DEBUG([](auto& f) {
			f << "Initalized PlayMusic";
		});
	}
	
	void PlayMusic::Execute() {
		OHS_DEBUG([](auto& f) {
			f << "playing music!";
		});
	}

	bool PlayMusic::IsFinished() {
		frc::Joystick& stick = Robot::Get().GetOI().GetDriverJoystick();
		const float deadBand = 0.2f;
		if (abs(stick.GetRawAxis(0)) > deadBand || abs(stick.GetRawAxis(1)) > deadBand || abs(stick.GetRawAxis(2)) > deadBand) {
			OHS_DEBUG([](auto& f) {
				f << "Quitting because joystick exceeds deadband!";
			});
			return true;
		}

		return false;
	}

	void PlayMusic::End(bool interrupted) {
		OHS_DEBUG([](auto& f) {
			f << "PlayMusic::End()";
		});
	}


}//namespace