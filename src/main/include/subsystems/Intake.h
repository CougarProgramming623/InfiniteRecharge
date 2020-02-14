#pragma once //PRARMA ONCE

#include <ctre/Phoenix.h>
#include <frc2/command/button/Button.h>
#include <frc2/command/RunCommand.h>

#include "Util.h"

namespace ohs2020{

class Intake {

public:
	Intake();

	void Init();
	void Spin();

private:

	WPI_TalonSRX intakeMotor;

	frc2::Button intakeButton;
};
}//namespace