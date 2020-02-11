#include "subsystems/Shooter.h"

#include <ctre/Phoenix.h>


namespace ohs2020{

const double DefaultShooterPower = 0;

Shooter::Shooter() : 

LemonSqueezer(35), 
LemonAID(100),
LemonAIDer( [&] { return m_OI.GetButtonBoard().GetRawButton(1); }),
FlyWheelToggle([&] { return m_OI.GetButtonBoard().GetRawButton(/*1*/ 100); }),
FlyWheelEncoder(35)

{ //2 -- Green Button | 19 -- Cargo/Hatch Toggle | 1 -- Vacuum Toggle Switch

}

void Shooter::Init() {

	SetFlyWheel();
	LemonShifter();

}

void Shooter::SetFlyWheel() {

	FlyWheelToggle.WhileHeld(frc2::RunCommand([&] {

		isFlywheelOn = true;
		flywheelWU = LemonSqueezer.GetSelectedSensorVelocity() / 2048;

		DebugOutF(std::to_string(flywheelWU));

		LemonSqueezer.Set(ControlMode::PercentOutput, DefaultShooterPower + m_OI.GetButtonBoard().GetRawAxis(0) /* * .75 */);
    
 	}, {} ));

	FlyWheelToggle.WhenReleased(frc2::InstantCommand([&] {

		isFlywheelOn = false;
		flywheelWU = 0;

		LemonSqueezer.Set(ControlMode::PercentOutput, 0);

	}, {} ));

}

void Shooter::LemonShifter() {

	LemonAIDer.WhenPressed(frc2::InstantCommand([&] {
		
		LemonAID.Set(ControlMode::PercentOutput, 0);

		std::this_thread::sleep_for(std::chrono::seconds(1));
		
		LemonAID.Set(ControlMode::PercentOutput, 0);

	}, {} ));
}


}//namespace