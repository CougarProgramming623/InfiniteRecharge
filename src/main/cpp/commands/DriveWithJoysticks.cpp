#include "commands/DriveWithJoysticks.h"
#include "Robot.h"

#include "ohs/Formatter.h"

#include <wpi/ArrayRef.h>
#include <frc2/command/PrintCommand.h>
#include <frc/DriverStation.h>
#include "Util.h"

namespace ohs2020 {

	Drive::Drive(){// : m_subsystem{driveTrain} { //m_subsystem{ Robot::Get().GetDriveTrain() }  
		AddRequirements(&Robot::Get().GetDriveTrain());

		frc2::CommandScheduler::GetInstance().Schedule(new frc2::PrintCommand("Has Requirement: " + GetRequirements().size()));
		frc2::CommandScheduler::GetInstance().Schedule(new frc2::PrintCommand("Ran Drive Contructor"));
	}

    void Drive::Initialize(){
		DebugOutF("IN INIT");
	}
	
	void Drive::Execute() {
		// DebugOutF("JOYSTICK");
		
		double y = Robot::Get().GetOI().GetDriverJoystick().GetY();
		double x = Robot::Get().GetOI().GetDriverJoystick().GetX();
		double rot = Robot::Get().GetOI().GetDriverJoystick().GetZ();
		double gyro = Robot::Get().GetNavX()->GetYaw();

		x = abs(x) <= 0.05f ? 0 : x;
		y = abs(y) <= 0.05f ? 0 : y;
		rot = abs(rot) <= 0.025f ? 0 : rot;

		ohs623::DefaultFormatter formatter;
		formatter << "Drive values: stick: [" << x << ", " << y << ", " << rot << "] Is FOD " << Robot::Get().GetOI().IsFOD();
		//frc::DriverStation::ReportError(formatter.c_str());

		if (Robot::Get().GetOI().IsFOD()) {
			Robot::Get().GetDriveTrain().CartesianDrive(-y, x, rot / 2, gyro);//Add in gyro later once the navax code is in
		} else {
			Robot::Get().GetDriveTrain().CartesianDrive(-y, x, rot / 2, 0.0);
		}
		
	}

	

	std::unique_ptr<frc2::Command> Drive::TransferOwnership() && {
		return std::unique_ptr<frc2::Command>(this);
	}


}//namespace