#include "subsystems/Shooter.h"
#include "Robot.h"
#include "ohs/RobotID.h"
#include "ohs/Log.h"

#include <frc/smartdashboard/SmartDashboard.h>

#include "frc2/command/FunctionalCommand.h"

using namespace ohs623;

namespace ohs2020{

const double DefaultShooterPower = 1;

Shooter::Shooter() : 

Flywheel(RobotID::GetID(FLYWHEEL)),
feeder(RobotID::GetID(FEEDER)),
launcher( [&] { return Robot::Get().GetOI().GetButtonBoard().GetRawButton(6); }), // Arm Override
flyWheelToggle([&] { return Robot::Get().GetOI().GetButtonBoard().GetRawButton(1); }), //Vacuum Toggle Switch
FlyWheelEncoder(RobotID::GetID(FLYWHEEL)),
timer()
{}

void Shooter::Init() {

	SetupShooterButtons();
	
}

inline void Shooter::SetupShooterButtons() {

	flyWheelToggle.WhileHeld(frc2::FunctionalCommand([this]{}, [this] { //on execute

		isFlywheelOn = true;
		flywheelWU = (int)((double)Flywheel.GetSelectedSensorVelocity() / 2048 * 600);
		DebugOutF(std::to_string(flywheelWU));
		frc::SmartDashboard::PutNumber("Flywheel Speed", flywheelWU);

		Flywheel.Set(ControlMode::PercentOutput, Robot::Get().GetOI().GetButtonBoard().GetRawAxis(0));

	}, [this] (bool f){//on end

		isFlywheelOn = false;

		Flywheel.Set(ControlMode::PercentOutput, 0);

	}, [this] { return false; }, {}));

	flyWheelToggle.WhenReleased(frc2::RunCommand([&] {

		flywheelWU = (int)((double)Flywheel.GetSelectedSensorVelocity() / 2048 * 600);
		DebugOutF(std::to_string(flywheelWU));

	}, {}));
} 
}//namespace