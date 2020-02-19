#include "subsystems/Climb.h"
#include "Robot.h"


namespace ohs2020 {

double BASIC_CLIMB_SPEED = 1;

Climb::Climb() : 

climbMotorLeft(4),
climbMotorRight(17)
//climbUp([&]         { return Robot::GetOI().GetButtonBoard().GetRawButton(5);   }),
//climbDown([&]       { return getOI().GetButtonBoard().GetRawButton(3);   }),
//climbLeft([&]       { return getOI().GetButtonBoard().GetRawButton(1); }),
//climbRight([&]      { return getOI().GetButtonBoard().GetRawButton(1); }),
//endgameOverride([&] { return getOI().GetButtonBoard().GetRawButton(4); }) 
{

	//climbMotorLeft.SetInverted(true);
	//climbMotorRight.SetInverted(false);

	//climbMotorLeft.SetNeutralMode(Brake);
	//climbMotorRight.SetNeutralMode(Brake);
  
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