#include "subsystems/Shooter.h"

namespace ohs2020{

const double DefaultShooterPower = 0;

Shooter::Shooter() : 

Flywheel(34), //35 
LemonAID(31),
launcher( [&] { return m_OI.GetButtonBoard().GetRawButton(2); }),
FlyWheelToggle([&] { return m_OI.GetButtonBoard().GetRawButton(1); }),
FlyWheelEncoder(34), //35
timer()
{ //2 -- Green Button | 19 -- Cargo/Hatch Toggle | 1 -- Vacuum Toggle Switch

}

void Shooter::Init() {

	SetFlyWheelCommands();

}

inline void Shooter::SetFlyWheelCommands() {

	FlyWheelToggle.WhileHeld(frc2::RunCommand([&] { FlyWheelOn(); }, {} ));
	FlyWheelToggle.WhenReleased(frc2::RunCommand([&] { FlyWheelOff(); }, {} ));

}


inline void Shooter::FlyWheelOn() {

	isFlywheelOn = true;
	flywheelWU = Flywheel.GetSelectedSensorVelocity() / 2048;

	//DebugOutF(std::to_string(m_OI.GetButtonBoard().GetRawAxis(0)));

	Flywheel.Set(ControlMode::PercentOutput, DefaultShooterPower + m_OI.GetButtonBoard().GetRawAxis(0) /* * .75 */);
}


inline void Shooter::FlyWheelOff() {

	isFlywheelOn = false;
	flywheelWU = 0;

	Flywheel.Set(ControlMode::PercentOutput, 0);
}


void Shooter::LoadLemon() {

	if(FlyWheelToggle.Get()){

		LemonAID.Set(ControlMode::PercentOutput, .1);

		timer.Reset();
		timer.Start();

		Wait(timer, 2); //Amount of time to load a lemon into the launching chamber

		LemonAID.Set(ControlMode::PercentOutput, 0);

	}
}

void Shooter::Shoot() {

	launcher.WhileHeld(frc2::RunCommand([&] {

		LoadLemon();

		Wait(timer, 3); //Amount of time needed for flywheel to reach required velocity again

	}, {} ));
}

void Shooter::Wait(frc2::Timer& timer, double time) {

	while(static_cast <double> (timer.Get()) < time);

}


}//namespace