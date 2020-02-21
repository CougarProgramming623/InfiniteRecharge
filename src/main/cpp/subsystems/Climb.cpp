#include "subsystems/Climb.h"
#include "Robot.h"
#include "ohs/RobotID.h"

#include <frc2/Timer.h>

#include <Robot.h>

using namespace ohs623;

namespace ohs2020 {

double BASIC_CLIMB_SPEED = 1;

Climb::Climb() : 

climbMotorLeft		(RobotID::GetID(CLIMB_LEFT)),
climbMotorRight		(RobotID::GetID(CLIMB_RIGHT)),

climbUp				(OHS_BUTTON(CLIMB_UP_ID)),
climbDown			(OHS_BUTTON(CLIMB_DOWN_ID)),
climbLeft			(OHS_BUTTON(CLIMB_LEFT_ID)),
climbRight			(OHS_BUTTON(CLIMB_RIGHT_ID)),
deployer			(OHS_BUTTON(DEPLOYER_ID)),
endgameOverride		(OHS_BUTTON(ENDGAME_OVERRIDE_ID)),
timer()
{

	climbMotorLeft.SetInverted(true);
	climbMotorRight.SetInverted(false);

	climbMotorLeft.SetNeutralMode(Brake);
	climbMotorRight.SetNeutralMode(Brake);
  
}

void Climb::Init() {
	OHS_DEBUG([](auto& f){ f << "Cilib::Init() 1"; });

	VerticalClimb();
	OHS_DEBUG([](auto& f){ f << "Cilib::Init() 2"; });

	//SideClimb();
	OHS_DEBUG([](auto& f){ f << "Cilib::Init() 3"; });

	//Deploy();
	OHS_DEBUG([](auto& f){ f << "Cilib::Init() 4"; });

}

void Climb::VerticalClimb() {
	OHS_DEBUG([](auto& f){ f << "Cilib::VerticalClimb() 1"; });

	climbUp.WhileHeld(frc2::RunCommand([&] {
		OHS_DEBUG([](auto& f){ f << "Cilib::WhileHeld Lambda() 1"; });

		if(CanClimb()){
			climbMotorLeft.Set(ControlMode::PercentOutput, BASIC_CLIMB_SPEED);
			climbMotorRight.Set(ControlMode::PercentOutput, BASIC_CLIMB_SPEED);

			DebugOutF("Climbing Up");
		}
		OHS_DEBUG([](auto& f){ f << "Cilib::WhileHeld Lambda() 2"; });

	}, {} ));
	OHS_DEBUG([](auto& f){ f << "Cilib::VerticalClimb() 2"; });

	climbUp.WhenReleased(frc2::InstantCommand([&] {

		climbMotorLeft.Set(ControlMode::PercentOutput, 0);
		climbMotorRight.Set(ControlMode::PercentOutput, 0);

	}, {} ));
	OHS_DEBUG([](auto& f){ f << "Cilib::VerticalClimb() 3"; });

	climbDown.WhenHeld(frc2::RunCommand([&] {

		if(CanClimb()){
			climbMotorLeft.Set(ControlMode::PercentOutput, -BASIC_CLIMB_SPEED);
			climbMotorRight.Set(ControlMode::PercentOutput, -BASIC_CLIMB_SPEED);

			DebugOutF("Climbing Down");
		}

	}, {} ));
	OHS_DEBUG([](auto& f){ f << "Cilib::VerticalClimb() 4"; });

	climbDown.WhenReleased(frc2::InstantCommand([&] {

		climbMotorLeft.Set(ControlMode::PercentOutput, 0);
		climbMotorRight.Set(ControlMode::PercentOutput, 0);

	}, {} ));
		OHS_DEBUG([](auto& f){ f << "Cilib::VerticalClimb() 5"; });

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

bool Climb::IsShot() {

	double pastVelocity[10];
	return false;
}
}//namespace