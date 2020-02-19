#pragma once

#include <ctre/Phoenix.h>
#include <frc2/command/RunCommand.h>
#include <frc2/command/Command.h>
#include <frc2/command/FunctionalCommand.h>

#include <frc2/Timer.h>

#include "Util.h"
#include "ohs/Button.h"

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

	bool IsShot();

private:

	WPI_TalonSRX climbMotorLeft;
	WPI_TalonSRX climbMotorRight;

	ohs623::Button climbUp;
	ohs623::Button climbDown;

	ohs623::Button climbLeft;
	ohs623::Button climbRight;

	ohs623::Button endgameOverride;
	ohs623::Button deployer;
	bool isDeployed = false;

	frc2::Timer timer;
};
}//namespace