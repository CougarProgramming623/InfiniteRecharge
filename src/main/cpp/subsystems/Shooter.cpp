#include "subsystems/Shooter.h"

#include <ctre/Phoenix.h>


namespace ohs2020{

const double DefaultShooterPower = 0;

Shooter::Shooter() : 

FlyWheel(35), 
FlyWheelToggle([&] { return m_OI.GetButtonBoard().GetRawButton(1); }),
FlyWheelEncoder(35)

{ //2 -- Green Button | 19 -- Cargo/Hatch Toggle | 1 -- Vacuum Toggle Switch

}

void Shooter::Init() {

	SetFlyWheel();

}

void Shooter::SetFlyWheel() {

 FlyWheelToggle.WhileHeld(frc2::InstantCommand([&] {

	isFlywheelOn = true;
	flywheelWU = FlyWheel.GetSelectedSensorVelocity() / 2048;

	DebugOutF(std::to_string(flywheelWU));

	FlyWheel.Set(ControlMode::PercentOutput, DefaultShooterPower + m_OI.GetButtonBoard().GetRawAxis(0) /* * .75 */);
    
 }, {} ));

 FlyWheelToggle.WhenReleased(frc2::InstantCommand([&] {

	isFlywheelOn = false;
	flywheelWU = 0;

	FlyWheel.Set(ControlMode::PercentOutput, 0);

 }, {} ));

FlyWheelToggle.WhenReleased(frc2::RunCommand([&] {

//DebugOutF(std::to_string(DefaultShooterPower + m_OI.GetButtonBoard().GetRawAxis(0) /* * .75 */ ));

 }, {} ));

}

}//namespace