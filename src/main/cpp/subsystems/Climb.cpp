#include "subsystems/Climb.h"

namespace ohs2020 {

Climb::Climb() : 

climbMotor(0),
deployer([&] { return getOI().GetButtonBoard().GetRawButton(0); }),
bigClimber([&] { return getOI().GetButtonBoard().GetRawButton(0); }) {}

void Climb::Init() {

Deploy();

}

void Climb::Deploy() {

deployer.WhenPressed(frc2::InstantCommand([&] {

	climbMotor.Set(ControlMode::PercentOutput, -0);

}, {} ));

}

void Climb::BigClimb() {

bigClimber.WhileHeld(frc2::InstantCommand([&] {

	climbMotor.Set(ControlMode::PercentOutput, 0);

}, {} ));

}

}//namespace