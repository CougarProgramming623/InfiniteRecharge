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

	bool GetFlywheelState() { return m_IsFlywheelOn; }

	double GetFlywheelWU() { return m_FlywheelWU; }

	WPI_TalonSRX* GetHighConveyor() { return &highConveyor; }
	WPI_TalonSRX* GetLowConveyor() { return &lowConveyor; }

	frc2::SequentialCommandGroup Shoot();


private:

	bool m_IsLoaded = false;
	bool m_IsFlywheelOn = false;
	double m_FlywheelWU = 0.0;

	WPI_TalonSRX m_Flywheel;
	WPI_TalonSRX m_Feeder;
	WPI_TalonSRX m_LowConveyor;
	WPI_TalonSRX m_HighConveyor;

	CANCoder m_FlyWheelEncoder;

	frc2::Button m_Launcher;
	frc2::Button m_FlyWheelToggle;
	frc2::Button m_ConveyorToggle;
	frc2::Button m_ReverseFeeder;


	frc2::Button m_BloopFeeder; 

	frc2::Timer timer;

	frc2::SequentialCommandGroup highGroup;
	frc2::SequentialCommandGroup lowGroup;

	int highCurrentCount = 0;
	int lowCurrentCount = 0;

};

}//namespace