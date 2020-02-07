#include "subsystems/Climb.h"



namespace ohs2020 {

Climb::Climb() : 

climbMotorLeft(0),
climbMotorRight(0),
climbUp([&] { return getOI().GetButtonBoard().GetRawButton(2); }),
climbDown([&] { return getOI().GetButtonBoard().GetRawButton(3); }),
climbLeft([&] { return getOI().GetButtonBoard().GetRawButton(100); }),
climbRight([&] { return getOI().GetButtonBoard().GetRawButton(100); }),
endgameOverride([&] { return getOI().GetButtonBoard().GetRawButton(4); }) {

	climbMotorLeft.SetInverted(true);
	climbMotorRight.SetInverted(false);

}

void Climb::Init() {

}

void Climb::VerticalClimb() {
climbUp.WhenHeld(
frc2::FunctionalCommand(
	{}, [this] {
		if(CanClimb()){
		climbMotorLeft.Set(ControlMode::PercentOutput, 0);
		climbMotorRight.Set(ControlMode::PercentOutput, 0);
		DebugOutF("Climbing Up");
		}
	}, {}, {}, {}
)
);

climbDown.WhenHeld(
frc2::FunctionalCommand(
	{}, [this] {
		if(CanClimb()){
		climbMotorLeft.Set(ControlMode::PercentOutput, -0);
		climbMotorRight.Set(ControlMode::PercentOutput, -0);
		DebugOutF("Climbing Down");
		}
	}, {}, {}, {}
)
);
}

void Climb::SideClimb() {
climbLeft.WhenHeld(
frc2::FunctionalCommand(
	{}, [this] {
		if(CanClimb()){
		climbMotorLeft.Set(ControlMode::PercentOutput, 0);
		climbMotorRight.Set(ControlMode::PercentOutput, 0);
		}
	}, {}, {}, {}
)
);

climbLeft.WhenHeld( 
frc2::FunctionalCommand(
	{}, [this] {
		if(CanClimb()){
		climbMotorLeft.Set(ControlMode::PercentOutput, 0);
		climbMotorRight.Set(ControlMode::PercentOutput, 0);
		}
	}, {}, {}, {}
)
);
}



bool Climb::CanClimb() {
if(endgameOverride.Get() || isEndgame)
	return true; 
else
	return false;
}
}//namespace