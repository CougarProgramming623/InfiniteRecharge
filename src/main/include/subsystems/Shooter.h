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

class Shooter : public frc2::SubsystemBase {

public:
	Shooter();

	void Init();

	void SetupShooterButtons();
	void SetupConveyorButtons();

	void ReverseConveyor();

	bool FlyWheelMode = false;

	bool GetFlywheelState() { return m_IsFlywheelOn; }

	double GetFlywheelWU() { return m_FlywheelWU; }

	WPI_TalonSRX& GetFeeder() { return m_Feeder; }
	WPI_TalonSRX& GetFlywheel() { return m_Flywheel; }
	WPI_TalonSRX& GetLowConveyor() { return m_LowConveyor; }
	WPI_TalonSRX& GetHighConveyor() { return m_HighConveyor; }

	frc2::SequentialCommandGroup Shoot(double);

	bool CheckVolt();

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
	
	std::array<double, 10> volt; 
	int index;
};

}//namespace