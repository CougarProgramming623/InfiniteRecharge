#pragma once

#include <ctre/Phoenix.h>
#include <frc2/command/button/Button.h>
#include <frc2/command/InstantCommand.h>

#include "OI.h"

namespace ohs2020{

const double FlyWheelDefaultSpeed = 0;

class Shooter {

public:
	Shooter();


	void Init();
	void SetFlyWheel();
	void FlyWheelTune();

	bool FlyWheelMode = false;
	double Tuner;

private:

WPI_TalonFX FlyWheel;
frc2::Button FlyWheelToggle;
frc2::Button FlyWheelTuner;

};

}//namespace