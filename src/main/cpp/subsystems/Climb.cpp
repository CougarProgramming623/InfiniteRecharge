#include "subsystems/Climb.h"
#include "Robot.h"

#include <frc2/Timer.h>

namespace ohs2020 {

double BASIC_CLIMB_SPEED = 1;

Climb::Climb() : 

climbMotorLeft(1), //4
climbMotorRight(6), //17
climbUp([&]         { return Robot::Get().GetOI().GetButtonBoard().GetRawButton(2);   }),
climbDown([&]       { return Robot::Get().GetOI().GetButtonBoard().GetRawButton(3);   }),
//climbLeft([&]       { return Robot::Get().GetOI().GetButtonBoard().GetRawButton(100); }),
//climbRight([&]      { return Robot::Get().GetOI().GetButtonBoard().GetRawButton(100); }),
deployer([&] 		{ return Robot::Get().GetOI().GetButtonBoard().GetRawButton(5);   }), // Drive Override Nuke Switch *Temporarily*
endgameOverride([&] { return Robot::Get().GetOI().GetButtonBoard().GetRawButton(4);   }),
timer()
{

	climbMotorLeft.SetInverted(true);
	climbMotorRight.SetInverted(false);

	climbMotorLeft.SetNeutralMode(Brake);
	climbMotorRight.SetNeutralMode(Brake);
  
}

void Climb::Init() {

VerticalClimb();
SideClimb();
Deploy();

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

climbLeft.WhenHeld(frc2::RunCommand([&] {

	if(CanClimb()){
		climbMotorLeft.Set(ControlMode::PercentOutput, BASIC_CLIMB_SPEED);
		climbMotorRight.Set(ControlMode::PercentOutput, BASIC_CLIMB_SPEED);
	}

}, {} ));

climbLeft.WhenReleased(frc2::InstantCommand([&] {

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


climbRight.WhenReleased(frc2::InstantCommand([&] {

	climbMotorLeft.Set(ControlMode::PercentOutput, 0);
	climbMotorRight.Set(ControlMode::PercentOutput, 0);

}, {} ));

}

void Climb::Deploy() {

	deployer.WhenPressed(frc2::FunctionalCommand( [this] { //on init

		timer.Reset();
		timer.Start();

		climbMotorLeft.Set(ControlMode::PercentOutput, .5);
		climbMotorRight.Set(ControlMode::PercentOutput, .5);

	}, [this] {}, [this] (bool f) { //on end

		climbMotorLeft.Set(ControlMode::PercentOutput, 0);
		climbMotorRight.Set(ControlMode::PercentOutput, 0);

		isDeployed = true;

	}, [this] { //is finished

		return timer.Get() > units::second_t(1); //Time to trigger gas spring

	}, {}));

}

inline bool Climb::CanClimb() {
	return (endgameOverride.Get() || (Timer::GetMatchTime() < 30)) && isDeployed;
}
}//namespace