#pragma once

#include <frc2/command/button/Button.h>
#include <ctre/Phoenix.h>
#include <frc2/command/RunCommand.h>
#include <frc2/command/Command.h>
#include <frc2/command/FunctionalCommand.h>

#include <frc/GenericHID.h>

#include <frc2/Timer.h>

#include "Util.h"

namespace ohs2020{

class Climb{

public:
	Climb();
	void Init();
	void RunDeploy();
	void Deploy();
	void VerticalClimb();
	void SideClimb();
	void LEDCanClimb();

	bool IsEndgame();
	bool CanClimb();

	bool isDeployed = false;
	bool isDeployFinished = false;
	
private:

	WPI_TalonSRX m_ClimbMotorLeft;
	WPI_TalonSRX m_ClimbMotorRight;

	frc2::Button m_ClimbUp;
	frc2::Button m_ClimbDown;
	frc2::Button m_ClimbLeft;
	frc2::Button m_ClimbRight;

	frc2::Button m_Deployer;
	frc2::Button m_EndgameOverride;

	frc2::Timer m_Timer;
};
}//namespace