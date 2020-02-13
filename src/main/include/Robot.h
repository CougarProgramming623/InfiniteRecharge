/*----------------------------------------------------------------------------*/
/* Copyright (c) 2017-2019 FIRST. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#pragma once

#include "OI.h"
#include "subsystems/DriveTrain.h"
#include "subsystems/Climb.h"
#include "subsystems/Shooter.h"
#include "subsystems/Intake.h"
#include "Util.h"

#include "ohs/Assert.h"

#include <AHRS.h>
#include <frc/TimedRobot.h>
#include <frc2/command/Command.h>

namespace ohs2020 {

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

	inline static Robot& Get() { return *s_Instance; } 

	inline OI& GetOI() { return m_oi; }
	inline DriveTrain& GetDriveTrain() { return m_DriveTrain; }
	inline Intake& GetIntake() { return m_intake; }
	inline AHRS* GetNavX() const { return navx; }
	inline bool IsInitComplete() const { return m_Init; }


private:
	// Have it null by default so that if testing teleop it
	// doesn't have undefined behavior and potentially crash.
	frc2::Command* m_autonomousCommand = nullptr;

	OI m_oi;
	DriveTrain m_DriveTrain;
	AHRS* navx;
	Climb m_climb;
	Shooter m_shooter;
	Intake m_intake;

	bool m_Init = false;

private:
	static Robot* s_Instance;
};

}//namespace
