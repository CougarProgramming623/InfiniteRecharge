#pragma once

#include <ctre/Phoenix.h>

#include <frc2/command/SubsystemBase.h>
#include <frc2/command/Command.h>	
#include <frc2/command/InstantCommand.h>
#include <frc2/command/RunCommand.h>

#include <frc2/Timer.h>

#include "Util.h"
#include "ohs/Button.h"

namespace ohs2020 {

const double FlyWheelDefaultSpeed = 0;

class Shooter : public frc2::SubsystemBase {

public:
	Shooter();

	void Init();

	void SetupShooterButtons();

	bool FlyWheelMode = false;

	bool GetFlywheelState() { return m_IsFlywheelOn; }

	double GetFlywheelWU() { return m_FlywheelWU; }

private:

	bool m_IsLoaded = false;
	bool m_IsFlywheelOn;
	double m_FlywheelWU;

	WPI_TalonSRX m_Flywheel;
	WPI_TalonSRX m_Feeder;

	frc2::Button m_Launcher;
	frc2::Button m_FlyWheelToggle;

	CANCoder m_FlyWheelEncoder;

	frc2::Timer m_Timer;
};

}//namespace