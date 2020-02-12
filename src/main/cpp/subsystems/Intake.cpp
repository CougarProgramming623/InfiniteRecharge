#include "subsystems/Intake.h"

namespace ohs2020{

Intake::Intake() :
intakeMotor(0),
intakeButton([&] { return GetOI().GetButtonBoard().GetRawButton(1); }) {

}

void Intake::Init() {

	Spin();

}

void Intake::Spin() {

intakeButton.WhileHeld( frc2::RunCommand( [&] {

DebugOutF("Pressed");

intakeMotor.Set(ControlMode::PercentOutput, 1);

}, {} ));


intakeButton.WhenReleased( frc2::InstantCommand( [&] {

DebugOutF("Released");

intakeMotor.Set(ControlMode::PercentOutput, 0);

}, {} ));
}

}//namespace