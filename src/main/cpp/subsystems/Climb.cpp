#include "subsystems/Climb.h"
#include "Robot.h"
#include "ohs/RobotID.h"

#include <frc2/Timer.h>

using namespace ohs623;

namespace ohs2020 {

double BASIC_CLIMB_SPEED = 1;

Climb::Climb() : 

climbMotorLeft		(RobotID::GetID(CLIMB_LEFT)),
climbMotorRight		(RobotID::GetID(CLIMB_RIGHT)), 
climbUp([&]         {return ButtonID CLIMB_UP_ID;}),
climbDown([&]       {return ButtonID CLIMB_DOWN_ID;}),
climbLeft([&]   	{return ButtonID CLIMB_LEFT_ID;}),
climbRight([&]  	{return ButtonID CLIMB_RIGHT_ID;}),
deployer([&] 		{return ButtonID DEPLOYER_ID;}),
endgameOverride([&] {return ButtonID ENDGAME_OVERRIDE_ID;}),
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