#pragma once

#include <ctre/Phoenix.h>
#include <frc2/command/button/Button.h>
#include <frc/DigitalInput.h>

namespace ohs2020{

class Intake {

public:
	Intake();

	void Init();
	void SetPositionButton();
	void SetToggleIntakeButtons();

	WPI_TalonSRX& GetPositioner() { return m_IntakePositioner; }
	WPI_TalonSRX& GetSpinner() { return m_Spinner; }

private:

frc2::Button m_IntakeDown;
frc2::Button m_IntakeStowed;
frc2::Button m_IntakeUp;

frc2::Button m_IntakeOn;
frc2::Button m_IntakeReverse;

WPI_TalonSRX m_IntakePositioner;
WPI_TalonSRX m_Spinner;

frc::DigitalInput m_MiddleLimit;
bool lastSwitch = false;

};
}//namespace