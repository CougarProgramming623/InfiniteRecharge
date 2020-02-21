#include "subsystems/Climb.h"
#include "Robot.h"
#include "ohs/RobotID.h"

#include <frc2/Timer.h>

using namespace ohs623;

namespace ohs2020 {

double BASIC_CLIMB_SPEED = 1;

Climb::Climb() : 

climbMotorLeft		( RobotID::GetID(CLIMB_LEFT)),
climbMotorRight		( RobotID::GetID(CLIMB_RIGHT)), 
climbUp([&]         { return Robot::Get().GetOI().GetButtonBoard().GetRawButton(19);  }),
climbDown([&]		{ return Robot::Get().GetOI().GetButtonBoard().GetRawButton(20);  }),
climbLeft([&]       { return Robot::Get().GetOI().GetButtonBoard().GetRawButton(21);  }),
climbRight([&]      { return Robot::Get().GetOI().GetButtonBoard().GetRawButton(22);  }),
deployer([&] 		{ return Robot::Get().GetOI().GetButtonBoard().GetRawButton(17);  }),
endgameOverride([&] { return Robot::Get().GetOI().GetButtonBoard().GetRawButton(18);  }),
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

		DebugOutF("Climbing Left");

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

		DebugOutF("Climbing Right");
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

		DebugOutF("Deploying **");

	}, [this] {}, [this] (bool f) { //on end

		climbMotorLeft.Set(ControlMode::PercentOutput, 0);
		climbMotorRight.Set(ControlMode::PercentOutput, 0);

		if(IsEndgame())
			isDeployed = true;

	}, [this] { //is finished

		if(!IsEndgame())
			return true;

		return timer.Get() > units::second_t(1); //Time to trigger gas spring

		if(IsEndgame()) DebugOutF("Deployed");

	}, {}));

}

void Climb::LEDCanClimb() {

if(CanClimb()) 
	Robot::Get().GetOI().GetButtonBoard().SetOutput(1, true);

}

inline bool Climb::IsEndgame() {
	return Timer::GetMatchTime() < 30 || endgameOverride.Get();
}

inline bool Climb::CanClimb() {
	return (Timer::GetMatchTime() < 30 && isDeployed) || endgameOverride.Get();
}

}//namespace