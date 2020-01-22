/*----------------------------------------------------------------------------*/
/* Copyright (c) 2017-2019 FIRST. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#pragma once

#include "OI.h"
#include "subsystems/DriveTrain.h"

#include <frc/TimedRobot.h>
#include <frc2/command/Command.h>

namespace ohs2020{


class Robot : public frc::TimedRobot {
public:
	Robot();

	void RobotInit() override;
	void RobotPeriodic() override;
	void DisabledInit() override;
	void DisabledPeriodic() override;
	void AutonomousInit() override;
	void AutonomousPeriodic() override;
	void TeleopInit() override;
	void TeleopPeriodic() override;
	void TestPeriodic() override;

public:

	static Robot& Get() { return *s_Instance; } 
	OI& GetOI() { return m_oi; }
	DriveTrain& GetDriveTrain() { return m_DriveTrain; }

private:
	// Have it null by default so that if testing teleop it
	// doesn't have undefined behavior and potentially crash.
	frc2::Command* m_autonomousCommand = nullptr;

	OI m_oi;
	DriveTrain m_DriveTrain;
private:
	static Robot* s_Instance;
	

};
}