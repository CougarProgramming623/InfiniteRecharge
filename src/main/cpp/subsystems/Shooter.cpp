#include "subsystems/Shooter.h"

namespace ohs2020{

const double DefaultShooterPower = 0;

Shooter::Shooter() : 

Flywheel(0), //35 
LemonAID(0),
launcher( [&] { return m_OI.GetButtonBoard().GetRawButton(6); }), // Arm	 Override
FlyWheelToggle([&] { return m_OI.GetButtonBoard().GetRawButton(1); }), //Vacuum Toggle Switch
FlyWheelEncoder(0) //35
{ //2 -- Green Button | 19 -- Cargo/Hatch Toggle | 1 -- Vacuum Toggle Switch

}

void Shooter::Init() {

	//SetFlyWheelCommands();
	//Shoot();
}

inline void Shooter::SetFlyWheelCommands() {

	//FlyWheelToggle.WhileHeld(frc2::RunCommand([&] { FlyWheelOn(); }, {} ));
	//FlyWheelToggle.WhenReleased(frc2::InstantCommand([&] { FlyWheelOff(); }, {} ));

}


inline void Shooter::FlyWheelOn() {

	DebugOutF("FlyWheel On");

	isFlywheelOn = true;
	flywheelWU = Flywheel.GetSelectedSensorVelocity() / 2048;

	//DebugOutF(std::to_string(m_OI.GetButtonBoard().GetRawAxis(0)));

	Flywheel.Set(ControlMode::PercentOutput, DefaultShooterPower + m_OI.GetButtonBoard().GetRawAxis(0) /* * .75 */);
}


inline void Shooter::FlyWheelOff() {

	DebugOutF("FlyWheel Off");

	isFlywheelOn = false;
	flywheelWU = 0;

	Flywheel.Set(ControlMode::PercentOutput, 0);
}


void Shooter::LoadLemon() {

	if(FlyWheelToggle.Get()){

		LemonAID.Set(ControlMode::PercentOutput, .1);

		Wait(2000); //Amount of time to load a lemon into the launching chamber

		LemonAID.Set(ControlMode::PercentOutput, 0);

	}
}

void Shooter::Shoot() {

	launcher.WhileHeld(frc2::RunCommand([&] {

		DebugOutF("Firing!");

		LoadLemon();

		Wait(3000); //Amount of time needed for flywheel to reach required velocity again

	}, {} ));
}
}//namespace