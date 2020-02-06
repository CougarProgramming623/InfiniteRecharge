#pragma once

#include <ctre/Phoenix.h>
#include <frc2/command/button/Button.h>
#include <frc2/command/InstantCommand.h>
#include <frc2/command/RunCommand.h>
#include <frc2/command/Command.h>

#include "Util.h"
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
	
	OI m_OI;

	bool GetFlywheelState() { return isFlywheelOn; }
	double GetFlywheelWU() { return flywheelWU; }

private:

	bool isFlywheelOn;
	double flywheelWU;

	WPI_TalonSRX FlyWheel;
	frc2::Button FlyWheelToggle;
	frc2::Button FlyWheelTuner;

	CANCoder FlyWheelEncoder;

};

}//namespace