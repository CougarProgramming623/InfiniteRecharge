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

	void SetFlyWheelCommands();
	void FlyWheelOn();
	void FlyWheelOff();

	void FlyWheelTune();
	void LoadLemon();

	void Shoot();

	bool FlyWheelMode = false;

	bool GetFlywheelState() { return isFlywheelOn; }
	double GetFlywheelWU() { return flywheelWU; }

	bool GetLoadStatus() { return isLoaded; }

private:

	bool isLoaded = false;
	bool isFlywheelOn;
	double flywheelWU;

	WPI_TalonSRX Flywheel;
	WPI_TalonSRX LemonAID;

	frc2::Button FlyWheelToggle;
	frc2::Button FlyWheelTuner;
	frc2::Button launcher;

	CANCoder FlyWheelEncoder;

	OI m_OI;
};

}//namespace