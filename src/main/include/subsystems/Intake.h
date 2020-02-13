#pragma once //PRARMA ONCE

#include <ctre/Phoenix.h>
#include <frc2/command/button/Button.h>
#include <frc2/command/RunCommand.h>


#include "OI.h"
#include "Util.h"

namespace ohs2020{

class Intake {

public:
	Intake();

	void Init();
	void Spin();

	OI& GetOI() { return m_oi; }

private:
	OI m_oi;

	WPI_TalonSRX intakeMotor;

	frc2::Button intakeButton;
};
}//namespace