#include "subsystems/Climb.h"
#include "Robot.h"
#include "ohs/RobotID.h"
#include "Util.h"

#include <frc2/Timer.h>
#include <frc2/command/SequentialCommandGroup.h>

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

	RemoveRegistry(&climbMotorLeft, &climbMotorRight);
  
}

void Climb::Init() {

isDeployed = false;
isDeployFinished = false;

VerticalClimb();
SideClimb();
Deploy();

}

void Climb::VerticalClimb() {

climbUp.WhileHeld(frc2::RunCommand([&] {

	if(CanClimb()){
		climbMotorLeft.Set(ControlMode::PercentOutput, -BASIC_CLIMB_SPEED);//correct direction
		climbMotorRight.Set(ControlMode::PercentOutput, -BASIC_CLIMB_SPEED);

		DebugOutF("Climbing Up");
	}

 }, {} ));

 climbUp.WhenReleased(frc2::InstantCommand([&] {

	climbMotorLeft.Set(ControlMode::PercentOutput, 0);
	climbMotorRight.Set(ControlMode::PercentOutput, 0);

 }, {} ));

 climbDown.WhenHeld(frc2::RunCommand([&] {

	if(CanClimb()){
		climbMotorLeft.Set(ControlMode::PercentOutput, BASIC_CLIMB_SPEED);
		climbMotorRight.Set(ControlMode::PercentOutput, BASIC_CLIMB_SPEED);//correct direction

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
		climbMotorRight.Set(ControlMode::PercentOutput, -BASIC_CLIMB_SPEED);

		DebugOutF("Climbing Left");

	}

}, {} ));

climbLeft.WhenReleased(frc2::InstantCommand([&] {

	climbMotorLeft.Set(ControlMode::PercentOutput, 0);
	climbMotorRight.Set(ControlMode::PercentOutput, 0);

}, {} ));


climbRight.WhenHeld(frc2::RunCommand([&] {
	
	if(CanClimb()){
		
		climbMotorLeft.Set(ControlMode::PercentOutput, -BASIC_CLIMB_SPEED);
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
     
		if(!isDeployed && IsEndgame()){
			DebugOutF("start deploy");
			timer.Reset();
			timer.Start();
			climbMotorLeft.Set(ControlMode::PercentOutput, 1); //correct direction
			climbMotorRight.Set(ControlMode::PercentOutput, -1);

			frc2::CommandScheduler::GetInstance().Schedule(new frc2::SequentialCommandGroup(
				frc2::WaitCommand(units::second_t(5)),
				frc2::InstantCommand( [&] { isDeployFinished = true; }, 
			{} )));
		}

		DebugOutF("Deploying **");

	}, [this] {}, [this] (bool f) { //on end

		if(IsEndgame()){
			climbMotorLeft.Set(ControlMode::PercentOutput, 0);
			climbMotorRight.Set(ControlMode::PercentOutput, 0);
			isDeployed = true;
		}
			

	}, [this] { //is finished

		if(isDeployed)
			return true;
		if(!IsEndgame()) 
			return true;

		DebugOutF("Deploying");

		return timer.Get() > units::second_t(.2); //Time to trigger gas spring

	}, {}));

}

void Climb::LEDCanClimb() {
	if(CanClimb()){
		Robot::Get().GetOI().GetButtonBoard().SetOutput(7, true);
	} else{
		Robot::Get().GetOI().GetButtonBoard().SetOutput(7, false);

	}
}

inline bool Climb::IsEndgame() {
	return (Timer::GetMatchTime() < 30 && Timer::GetMatchTime() != -1) ||  endgameOverride.Get();
}

inline bool Climb::CanClimb() {
	return ((Timer::GetMatchTime() < 30 && Timer::GetMatchTime() != -1 ) && isDeployFinished) || endgameOverride.Get();
}

}//namespace