#include "commands/DriveWithJoysticks.h"
#include "Robot.h"

#include "ohs/Formatter.h"

#include <frc/DriverStation.h>

namespace ohs2020 {

	Drive::Drive() {
		AddRequirements({ &Robot::Get().GetDriveTrain() });
	}

	void Drive::Execute() {

		double y = Robot::Get().GetOI().GetDriverJoystick().GetY();
		double x = Robot::Get().GetOI().GetDriverJoystick().GetX();
		double rot = Robot::Get().GetOI().GetDriverJoystick().GetZ();
		//double gyro = Robot::navx->GetYaw();

		x = abs(x) <= 0.075f ? 0 : x;
		y = abs(y) <= 0.075f ? 0 : y;
		rot = abs(rot) <= 0.05f ? 0 : rot;

		ohs623::DefaultFormatter formatter;
		formatter << "Drive values: stick: [" << x << ", " << y << ", " << rot << "] Is FOD " << Robot::Get().GetOI().IsFOD();
		frc::DriverStation::ReportError(formatter.c_str());


		if (Robot::Get().GetOI().IsFOD()) {
			Robot::Get().GetDriveTrain().CartesianDrive(x, y, rot / 2, 0.0);//Add in gryo later once the navax code is in
		} else {
			Robot::Get().GetDriveTrain().CartesianDrive(x, y, rot / 2, 0.0);
		}
	}

	std::unique_ptr<frc2::Command> Drive::TransferOwnership() && {
		return std::unique_ptr<frc2::Command>(this);
	}


}//namespace
