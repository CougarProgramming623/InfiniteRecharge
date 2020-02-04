#include "subsystems/Shooter.h"

namespace ohs2020{

const double DefaultShooterPower = .5;

Shooter::Shooter() : 

FlyWheel(6), 
FlyWheelToggle([&] { return m_OI.GetButtonBoard().GetRawButton(2); }),
FlyWheelTuner([&] { return m_OI.GetDriverJoystick().GetRawButton(100); }) {

}

void Shooter::Init() {

	SetFlyWheel();

}

void Shooter::SetFlyWheel() {

FlyWheelToggle.WhenPressed(frc2::InstantCommand([&] {

	DebugOutF("Pressed FlyWheel");
	FlyWheelMode = !FlyWheelMode;

	if (!FlyWheelMode) 
		FlyWheel.Set(ControlMode::PercentOutput, 0);

}, {} ));

 FlyWheelToggle.WhenPressed(frc2::RunCommand([&] {
	 
	if(FlyWheelMode)
		FlyWheel.Set(ControlMode::PercentOutput, DefaultShooterPower + m_OI.GetButtonBoard().GetRawAxis(0) * .75);

 }, {} ));

}

}//namespace