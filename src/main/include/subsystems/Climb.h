#pragma once

#include <frc2/command/button/Button.h>
#include <ctre/Phoenix.h>
#include <frc2/command/InstantCommand.h>


#include "OI.h"

namespace ohs2020{

class Climb{

public:
	Climb();
	void Init();
	void Deploy();
	void BigClimb();


	OI& getOI() { return oi; }

private:
	OI oi;

	WPI_TalonSRX climbMotor;

	frc2::Button deployer;
	frc2::Button bigClimber;
};
}//namespace