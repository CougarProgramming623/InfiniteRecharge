#pragma once

#include <frc2/command/button/Button.h>
#include <ctre/Phoenix.h>
#include <frc2/command/RunCommand.h>
#include <frc2/command/Command.h>
#include <frc2/command/FunctionalCommand.h>

#include <frc2/Timer.h>

#include "Util.h"

namespace ohs2020{

class Climb{

public:
	Climb();
	void Init();
	void Deploy();
	bool CanClimb();
	void VerticalClimb();
	void SideClimb();
	bool IsDeployed();

private:

	WPI_TalonSRX climbMotorLeft;
	WPI_TalonSRX climbMotorRight;

	frc2::Button climbUp;
	frc2::Button climbDown;

	frc2::Button climbLeft;
	frc2::Button climbRight;

	frc2::Button endgameOverride;
	frc2::Button deployer;
	bool isDeployed = false;

	frc2::Timer timer;
};
}//namespace