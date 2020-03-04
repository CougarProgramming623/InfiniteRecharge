#include "subsystems/Intake.h"

#include "ohs/RobotID.h"

#include <frc2/command/InstantCommand.h>

using namespace ohs623;

namespace ohs2020{

Intake::Intake() :

m_IntakeDown([&]	{ return Robot::Get().GetOI().GetDriverJoystick().GetRawButton(2);}),

m_IntakeOn([&] 		{ return Robot::Get().GetOI().GetDriverJoystick().GetRawButton(2);}),
m_IntakeOff([&] 	{ return Robot::Get().GetOI().GetDriverJoystick().GetRawButton(2);}),
m_IntakeReverse([&] { return Robot::Get().GetOI().GetDriverJoystick().GetRawButton(2);}),

m_IntakePositioner	(RobotID::GetID(INTAKE_POSITIONER)),
m_MainIntakeMotor	(RobotID::GetID(INTAKE_MAIN)) 
{}

int POS_FORWARD = 1;
int POS_BACKWARDS = -1;

int MAIN_FORWARD = 1;
int MAIN_BACKWARDS = -1;

void Intake::Init() {
	m_IntakePositioner.Set(ControlMode::PercentOutput, POS_FORWARD);

	SetPositionButton();
}

void Intake::SetPositionButton() {

	m_IntakeDown.WhenPressed(frc2::InstantCommand([&] 	{ m_IntakePositioner.Set(ControlMode::PercentOutput, POS_FORWARD	); }, {}));
	m_IntakeDown.WhenReleased(frc2::InstantCommand([&] 	{ m_IntakePositioner.Set(ControlMode::PercentOutput, POS_BACKWARDS	); }, {}));

}

void Intake::SetToggleIntakeButtons() {

	m_IntakeOn.WhenPressed(frc2::InstantCommand([&] 		{ m_MainIntakeMotor.Set(ControlMode::PercentOutput, MAIN_FORWARD);	}, {}));
	m_IntakeOff.WhenPressed(frc2::InstantCommand([&] 		{ m_MainIntakeMotor.Set(ControlMode::PercentOutput, 0);				}, {}));
	m_IntakeReverse.WhenPressed(frc2::InstantCommand([&] 	{ m_MainIntakeMotor.Set(ControlMode::PercentOutput, MAIN_BACKWARDS);}, {}));

}
}//namespace