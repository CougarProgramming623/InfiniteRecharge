#pragma once

#include <frc2/command/button/Button.h>
#include <ctre/Phoenix.h>
#include <frc2/command/InstantCommand.h>

#include "Util.h"
#include "OI.h"

namespace ohs2020{

class Climb{

public:
	Climb();
	void Init();
	void Deploy();
	void BigClimb();
	bool CanClimb();


	OI& getOI() { return oi; }

	bool isEndgame = false;

private:
	OI oi;

	WPI_TalonSRX climbMotor;

	frc2::Button deployer;
	frc2::Button climbUp;
	frc2::Button climbDown;
	frc2::Button endgameOverride;
};
}//namespace