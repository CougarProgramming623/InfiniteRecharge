#pragma once

#include <ctre/Phoenix.h>
#include <frc2/command/button/Button.h>

#include <frc2/command/SubsystemBase.h>
#include <frc2/command/Command.h>	
#include <frc2/command/InstantCommand.h>
#include <frc2/command/RunCommand.h>

#include <frc2/Timer.h>

#include "Util.h"

namespace ohs2020{

const double FlyWheelDefaultSpeed = 0;

class Shooter : public frc2::SubsystemBase {

public:
	Shooter();

	void Init();

	void SetupShooterButtons();

	bool FlyWheelMode = false;

	bool GetFlywheelState() { return isFlywheelOn; }

	double GetFlywheelWU() { return flywheelWU; }

private:

	bool isLoaded = false;
	bool isFlywheelOn;
	double flywheelWU;

	WPI_TalonSRX Flywheel;
	WPI_TalonSRX feeder;

	frc2::Button flyWheelToggle;
	frc2::Button FlyWheelTuner;
	frc2::Button launcher;

	CANCoder FlyWheelEncoder;

	frc2::Timer timer;
};

}//namespace