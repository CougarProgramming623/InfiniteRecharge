#include "subsystems/Intake.h"

#include "ohs/RobotID.h"

#include <frc2/command/InstantCommand.h>
#include <frc2/command/FunctionalCommand.h>
#include<frc/DigitalInput.h>

using namespace ohs623;

namespace ohs2020{

Intake::Intake() :

m_IntakeDown([&]		{ return Robot::Get().GetOI().GetButtonBoard().GetRawButton(10);}),
m_IntakeStowed([&]   	{ return Robot::Get().GetOI().GetButtonBoard().GetRawButton(9);}),
m_IntakeUp([&]			{ return !Robot::Get().GetOI().GetButtonBoard().GetRawButton(10) && !Robot::Get().GetOI().GetButtonBoard().GetRawButton(9);}),

m_IntakeOn([&] 			{ return Robot::Get().GetOI().GetButtonBoard().GetRawButton(11);}),
m_IntakeOff([&] 		{ return Robot::Get().GetOI().GetDriverJoystick().GetRawButton(2);}),
m_IntakeReverse([&] 	{ return Robot::Get().GetOI().GetButtonBoard().GetRawButton(6);}),

m_IntakePositioner	(RobotID::GetID(INTAKE_POSITIONER)),
m_MainIntakeMotor	(RobotID::GetID(INTAKE_SPIN)),

m_MiddleLimit(0){

m_IntakePositioner.SetNeutralMode(Brake);

}

// int POS_FORWARD = .2;
// int POS_BACKWARDS = .2;

// int MAIN_FORWARD = 1;
// int MAIN_BACKWARDS = -1;

void Intake::Init() {
	// m_IntakePositioner.Set(ControlMode::PercentOutput, POS_FORWARD);
    SetToggleIntakeButtons();
	SetPositionButton();
	m_IntakePositioner.SetInverted(true);
}

void Intake::SetPositionButton() {
	m_IntakeDown.WhileHeld(frc2::RunCommand([&] {
		 m_IntakePositioner.Set(ControlMode::PercentOutput, -1);
		 DebugOutF("DOWN");
	},{}));
	m_IntakeStowed.WhileHeld(frc2::RunCommand([&] {
		m_IntakePositioner.Set(ControlMode::PercentOutput, 1);
		DebugOutF("UP");
	},{}));


	m_IntakeUp.WhenHeld(frc2::FunctionalCommand( [this] { //on init
		DebugOutF("MIDDLE");
		lastSwitch = m_IntakePositioner.IsFwdLimitSwitchClosed();
	}, [this] {
		if(lastSwitch){
			m_IntakePositioner.Set(ControlMode::PercentOutput, -1);
		} else {
			m_IntakePositioner.Set(ControlMode::PercentOutput, 1);
		}
	}, [this] (bool f) { //on end
        m_IntakePositioner.Set(ControlMode::PercentOutput, 0);
	}, [this] { //is finished
		DebugOutF(std::to_string(!m_MiddleLimit.Get()));
		return !m_MiddleLimit.Get();
	}, {}));
}

void Intake::SetToggleIntakeButtons() {

	m_IntakeOn.WhileHeld(frc2::RunCommand([&] 		{ m_MainIntakeMotor.Set(ControlMode::PercentOutput, 1);	}, {}));
	m_IntakeOn.WhenReleased(frc2::InstantCommand([&] 		{ m_MainIntakeMotor.Set(ControlMode::PercentOutput, 0);	}, {}));
	
	// m_IntakeOff.WhenPressed(frc2::InstantCommand([&] 		{ m_MainIntakeMotor.Set(ControlMode::PercentOutput, 0);				}, {}));
	// m_IntakeReverse.WhenPressed(frc2::InstantCommand([&] 	{ m_MainIntakeMotor.Set(ControlMode::PercentOutput, .5);}, {}));

}
}//namespace