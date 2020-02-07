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

DebugOutF("Spin() Run");
/*
intakeButton.WhenHeld(
frc2::FunctionalCommand(
	{}, [this] {
		intakeMotor.Set(ControlMode::PercentOutput, 0);
		DebugOutF("Climbing");
	}, {}, {}, {}
));
*/
intakeButton.WhenPressed(frc2::InstantCommand([&] {

DebugOutF("Pressed");

}, {} ));

}

}//namespace