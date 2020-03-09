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

	void ReverseConveyor();

	bool FlyWheelMode = false;

	bool GetFlywheelState() { return isFlywheelOn; }

	double GetFlywheelWU() { return flywheelWU; }

	frc2::SequentialCommandGroup Shoot(double);

	bool CheckVolt();

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
	frc2::Button m_TurnShoot;

	frc2::Button reverseFeeder;

	frc2::Button m_BloopFeeder; 

	CANCoder FlyWheelEncoder;

	frc2::Timer timer;
	
	std::array<double, 10> volt; 
	int index;
};

}//namespace