#include "subsystems/Intake.h"
#include "Robot.h"


namespace ohs2020{

Intake::Intake() :
intakeMotor(0),
intakeButton([&] { return Robot::Get().GetOI().GetButtonBoard().GetRawButton(7); }) { //Fork Override Button (yellow)

}

void Intake::Init() {

	Spin();

}

void Intake::Spin() {

intakeButton.WhileHeld( frc2::RunCommand( [&] {

DebugOutF("Intaking");

intakeMotor.Set(ControlMode::PercentOutput, 1);

}, {} ));


intakeButton.WhenReleased( frc2::InstantCommand( [&] {

DebugOutF("Not Intaking");

intakeMotor.Set(ControlMode::PercentOutput, 0);

}, {} ));
}

}//namespace