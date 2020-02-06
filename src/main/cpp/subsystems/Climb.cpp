#include "subsystems/Climb.h"

namespace ohs2020 {

Climb::Climb() : 

climbMotor(0),
climbUp([&] { return getOI().GetButtonBoard().GetRawButton(2); }),
climbDown([&] { return getOI().GetButtonBoard().GetRawButton(3); }),
endgameOverride([&] { return getOI().GetButtonBoard().GetRawButton(4); }) {}

void Climb::Init() {

Deploy();
BigClimb();

}

void Climb::Deploy() {

deployer.WhenPressed(frc2::InstantCommand([&] {
	climbMotor.Set(ControlMode::PercentOutput, -0);
}, {} ));

}

void Climb::BigClimb() {

DebugOutF("BigClimb() Run");

climbDown.WhileHeld(frc2::InstantCommand([&] {

if(CanClimb()){
	climbMotor.Set(ControlMode::PercentOutput, -0);
	DebugOutF("Climbing Down");
}
}, {} ));


climbUp.WhileHeld(frc2::InstantCommand([&] {
if(CanClimb()){
	climbMotor.Set(ControlMode::PercentOutput, 0);
	DebugOutF("Climbing Up");
}
}, {} ));
}

bool Climb::CanClimb() {
if(endgameOverride.Get() || isEndgame)
	return true; 
else
	return false;
}
}//namespace