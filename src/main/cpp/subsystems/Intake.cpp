#include "subsystems/Intake.h"

#include "ohs/RobotID.h"

#include <frc2/command/InstantCommand.h>
#include <frc2/command/FunctionalCommand.h>
#include<frc/DigitalInput.h>

using namespace ohs623;

namespace ohs2020{

Intake::Intake() :

m_IntakeDown([&]		{ return Robot::Get().GetOI().GetDriverJoystick().GetRawButton(2);}),
m_IntakeStowed([&]   	{ return Robot::Get().GetOI().GetDriverJoystick().GetRawButton(-1);}),
m_IntakeUp([&]			{ return !Robot::Get().GetOI().GetDriverJoystick().GetRawButton(2) && !Robot::Get().GetOI().GetDriverJoystick().GetRawButton(-1);}),

m_IntakeOn([&] 			{ return Robot::Get().GetOI().GetDriverJoystick().GetRawButton(2);}),
m_IntakeOff([&] 		{ return Robot::Get().GetOI().GetDriverJoystick().GetRawButton(2);}),
m_IntakeReverse([&] 	{ return Robot::Get().GetOI().GetDriverJoystick().GetRawButton(2);}),

m_IntakePositioner	(RobotID::GetID(INTAKE_POSITIONER)),
m_MainIntakeMotor	(RobotID::GetID(INTAKE_MAIN)),

input(frc::DigitalInput(0))

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

	m_IntakeDown.WhileHeld(frc2::RunCommand([&] { m_IntakePositioner.Set(ControlMode::PercentOutput, POS_FORWARD);},{}));
	m_IntakeStowed.WhileHeld(frc2::RunCommand([&] {m_IntakePositioner.Set(ControlMode::PercentOutput, POS_BACKWARDS);},{}));
	m_IntakeUp.WhileHeld(frc2::FunctionalCommand( [this] { //on init
		if(m_IntakePositioner.IsFwdLimitSwitchClosed()){
			m_IntakePositioner.Set(ControlMode::PercentOutput, POS_BACKWARDS);
		}else
		{
			m_IntakePositioner.Set(ControlMode::PercentOutput, POS_FORWARD);
		}
		
	}, [this] {}, [this] (bool f) { //on end
        m_IntakePositioner.Set(ControlMode::PercentOutput, 0);
	}, [this] { //is finished
		return input.Get();
	}, {}));
}

void Intake::SetToggleIntakeButtons() {

	m_IntakeOn.WhenPressed(frc2::InstantCommand([&] 		{ m_MainIntakeMotor.Set(ControlMode::PercentOutput, MAIN_FORWARD);	}, {}));
	m_IntakeOff.WhenPressed(frc2::InstantCommand([&] 		{ m_MainIntakeMotor.Set(ControlMode::PercentOutput, 0);				}, {}));
	m_IntakeReverse.WhenPressed(frc2::InstantCommand([&] 	{ m_MainIntakeMotor.Set(ControlMode::PercentOutput, MAIN_BACKWARDS);}, {}));

}
}//namespace