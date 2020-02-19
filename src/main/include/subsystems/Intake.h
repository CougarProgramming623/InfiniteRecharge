#pragma once //PRARMA ONCE

#include <ctre/Phoenix.h>
#include <frc2/Timer.h>

#include "Util.h"
#include "ohs/Button.h"


namespace ohs2020 {

class Intake {

public:
	Intake();

	void Init();

	void IntakeToggleCommands();
	void IntakePlacementCommands();

	void IntakePlacementUp();
	void IntakePlacementDown();
	void ManualModeCommands();

private:

	bool intakeOn = false;

	WPI_TalonSRX intakeMotor;
	WPI_TalonSRX intakeLift;

	ohs623::Button intakeDownButton;
	ohs623::Button intakeUpButton;
	ohs623::Button intakeManualButton;
	ohs623::Button intakeOnButton;

	frc2::Timer timer;

};
}//namespace