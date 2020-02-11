#pragma once

#include <ctre/Phoenix.h>
#include <frc2/command/button/Button.h>

#include <frc2/command/InstantCommand.h>
#include <frc2/command/RunCommand.h>
#include <frc2/command/Command.h>	
#include <frc2/command/WaitCommand.h>

#include "Util.h"
#include "OI.h"

#include <chrono>

namespace ohs2020{

const double FlyWheelDefaultSpeed = 0;

class Shooter {

public:
	Shooter();

	void Init();
	void SetFlyWheel();
	void FlyWheelTune();
	void LemonShifter();
	void Wait(frc2::Timer& timer, double time);

	bool FlyWheelMode = false;
	
	OI m_OI;

	bool GetFlywheelState() { return isFlywheelOn; }
	double GetFlywheelWU() { return flywheelWU; }
	frc2::Timer& GetTimer() { return shooterTimer; }

private:

	bool isFlywheelOn;
	double flywheelWU;

	WPI_TalonSRX LemonSqueezer;
	WPI_TalonSRX LemonAID;

	frc2::Button FlyWheelToggle;
	frc2::Button FlyWheelTuner;
	frc2::Button LemonAIDer;

	CANCoder FlyWheelEncoder;

	frc2::Timer shooterTimer;

};

}//namespace