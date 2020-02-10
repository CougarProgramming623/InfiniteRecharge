#include "subsystems/Climb.h"



namespace ohs2020 {

int BASIC_CLIMB_SPEED;

Climb::Climb() : 

climbMotorLeft(0),
climbMotorRight(0),
climbUp([&]         { return getOI().GetButtonBoard().GetRawButton(2);   }),
climbDown([&]       { return getOI().GetButtonBoard().GetRawButton(3);   }),
climbLeft([&]       { return getOI().GetButtonBoard().GetRawButton(100); }),
climbRight([&]      { return getOI().GetButtonBoard().GetRawButton(100); }),
endgameOverride([&] { return getOI().GetButtonBoard().GetRawButton(4); }) {

	climbMotorLeft.SetInverted(true);
	climbMotorRight.SetInverted(false);

}

void Climb::Init() {

VerticalClimb();
SideClimb();

}

void Climb::VerticalClimb() {

climbUp.WhileHeld(frc2::RunCommand([&] {

	if(CanClimb()){
		climbMotorLeft.Set(ControlMode::PercentOutput, BASIC_CLIMB_SPEED);
		climbMotorRight.Set(ControlMode::PercentOutput, BASIC_CLIMB_SPEED);

		DebugOutF("Climbing Up");
	}

 }, {} ));

 climbUp.WhenReleased(frc2::InstantCommand([&] {

	climbMotorLeft.Set(ControlMode::PercentOutput, 0);
	climbMotorRight.Set(ControlMode::PercentOutput, 0);

 }, {} ));

 climbDown.WhenHeld(frc2::RunCommand([&] {

	if(CanClimb()){
		climbMotorLeft.Set(ControlMode::PercentOutput, -BASIC_CLIMB_SPEED);
		climbMotorRight.Set(ControlMode::PercentOutput, -BASIC_CLIMB_SPEED);

		DebugOutF("Climbing Down");
	}

 }, {} ));

 climbDown.WhenReleased(frc2::InstantCommand([&] {

	climbMotorLeft.Set(ControlMode::PercentOutput, 0);
	climbMotorRight.Set(ControlMode::PercentOutput, 0);

 }, {} ));
}


void Climb::SideClimb() {

climbLeft.WhenHeld(frc2::InstantCommand([&] {

	if(CanClimb()){
		//Modify one speed later
		climbMotorLeft.Set(ControlMode::PercentOutput, BASIC_CLIMB_SPEED);
		climbMotorRight.Set(ControlMode::PercentOutput, BASIC_CLIMB_SPEED);
	}

}, {} ));

climbLeft.WhenReleased(frc2::RunCommand([&] {

	climbMotorLeft.Set(ControlMode::PercentOutput, 0);
	climbMotorRight.Set(ControlMode::PercentOutput, 0);

}, {} ));


climbRight.WhenHeld(frc2::RunCommand([&] {
	
	if(CanClimb()){
		//Modify one speed later
		climbMotorLeft.Set(ControlMode::PercentOutput, BASIC_CLIMB_SPEED);
		climbMotorRight.Set(ControlMode::PercentOutput, BASIC_CLIMB_SPEED);
	}

 }, {} ));


climbRight.WhenReleased(frc2::RunCommand([&] {

	climbMotorLeft.Set(ControlMode::PercentOutput, 0);
	climbMotorRight.Set(ControlMode::PercentOutput, 0);

}, {} ));

}

bool Climb::CanClimb() {
if(endgameOverride.Get() || isEndgame)
	return true; 
else
	return false;
}
}//namespace