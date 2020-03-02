#pragma once

#include <ctre/Phoenix.h>
#include <frc2/command/button/Button.h>

#include <frc2/command/SubsystemBase.h>
#include <frc2/command/Command.h>	
#include <frc2/command/InstantCommand.h>
#include <frc2/command/RunCommand.h>
#include <frc2/command/SequentialCommandGroup.h>

#include <frc2/Timer.h>

#include "Util.h"

namespace ohs2020{

const double FlyWheelDefaultSpeed = 0;

class Shooter : public frc2::SubsystemBase {

public:
	Shooter();

	void Init();

	void SetupShooterButtons();
	void SetupConveyorButtons();

	bool CheckHighCurrent();

	void SetHighCurrentCount(int newCurrentCount) { highCurrentCount = newCurrentCount; }

	int GetHighCurrentCount() { return highCurrentCount; }

	bool CheckLowCurrent();

	void SetLowCurrentCount(int newCurrentCount) { lowCurrentCount = newCurrentCount; }

	int GetLowCurrentCount() { return lowCurrentCount; }

	bool FlyWheelMode = false;

	bool GetFlywheelState() { return isFlywheelOn; }

	double GetFlywheelWU() { return flywheelWU; }

	WPI_TalonSRX* GetHighConveyor() { return &highConveyor; }
	WPI_TalonSRX* GetLowConveyor() { return &lowConveyor; }

	frc2::SequentialCommandGroup Shoot();


private:

	bool isLoaded = false;
	bool isFlywheelOn;
	double flywheelWU;

	WPI_TalonSRX Flywheel;
	WPI_TalonSRX feeder;
	WPI_TalonSRX lowConveyor;
	WPI_TalonSRX highConveyor;

	frc2::Button flyWheelToggle;
	frc2::Button FlyWheelTuner;
	frc2::Button launcher;
	frc2::Button conveyorToggle;

	frc2::Button reverseFeeder;

	CANCoder FlyWheelEncoder;

	frc2::Timer timer;

	frc2::SequentialCommandGroup highGroup;
	frc2::SequentialCommandGroup lowGroup;

	int highCurrentCount = 0;
	int lowCurrentCount = 0;

	
};

}//namespace