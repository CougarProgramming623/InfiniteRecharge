#pragma once //PRARMA ONCE

#include <ctre/Phoenix.h>
#include <frc2/command/button/Button.h>

#include "Util.h"

#include <frc2/Timer.h>

namespace ohs2020{

class Intake {

public:
	Intake();

	void Init();

	void IntakeToggleCommands();
	void IntakePlacementCommands();

	void IntakePlacementUp();
	void IntakePlacementDown();

private:

	bool intakeOn = false;

	WPI_TalonSRX intakeMotor;
	WPI_TalonSRX intakeLift;

	frc2::Button intakeDownButton;
	frc2::Button intakeUpButton;
	frc2::Button intakeManualButton;
	frc2::Button intakeOnButton;

	frc2::Timer timer;

};
}//namespace