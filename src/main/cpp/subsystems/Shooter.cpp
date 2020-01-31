#include "subsystems/Shooter.h"

namespace ohs2020{

Shooter::Shooter() : 

FlyWheel(0), 
FlyWheelTuner([&] { return OI::GetButtonBoard().GetRawButton(0);}){
}

void Shooter::Init() {

	SetFlyWheel();

}



void Shooter::SetFlyWheel() {

FlyWheelToggle.WhenPressed(frc2::InstantCommand([&] {
	FlyWheelMode = !FlyWheelMode;

	if (FlyWheelMode) {
		FlyWheel.Set(ControlMode::Velocity, FlyWheelDefaultSpeed - Tuner);
	}else {
 		FlyWheel.Set(ControlMode::Velocity, 0); 
	}
 
}, {} ));

}


void Shooter::FlyWheelTune() {


}

}//namespace