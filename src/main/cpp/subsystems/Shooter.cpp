#include "subsystems/Shooter.h"
#include "Robot.h"
#include "ohs/RobotID.h"
#include "ohs/Log.h"
#include "Util.h"

#include <frc/smartdashboard/SmartDashboard.h>
#include <frc/smartdashboard/SendableRegistry.h>

#include "frc2/command/FunctionalCommand.h"

using namespace ohs623;

namespace ohs2020{

const double DefaultShooterPower = 1;

Shooter::Shooter() : 

Flywheel(RobotID::GetID(FLYWHEEL)),
feeder(RobotID::GetID(FEEDER)),
lowTransport(RobotID::GetID(LOW_TRANSPORT)),
highTransport(RobotID::GetID(HIGH_TRANSPORT)),
FlyWheelEncoder(RobotID::GetID(FLYWHEEL)),

launcher( [&] 		{ return Robot::Get().GetOI().GetButtonBoard().GetRawButton(2); 	}),
flyWheelToggle([&] 	{ return Robot::Get().GetOI().GetButtonBoard().GetRawButton(11);	}), 
conveyorToggle( [&] { return Robot::Get().GetOI().GetButtonBoard().GetRawButton(15); 		}),
timer() {

	RemoveRegistry(this, &Flywheel, &feeder, &lowTransport, &highTransport);

}

void Shooter::Init() {

	SetupShooterButtons();
	SetupTransportButtons();

}

void Shooter::SetupShooterButtons() {

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


	launcher.WhenHeld(frc2::RunCommand([&] {

		feeder.Set(ControlMode::PercentOutput, 1);

	}, {} ));

	launcher.WhenReleased(frc2::InstantCommand([&] {

		feeder.Set(ControlMode::PercentOutput, 0);

	}, {} ));
} 

void Shooter::SetupTransportButtons() {

conveyorToggle.WhenHeld(frc2::RunCommand([&] {

	lowTransport.Set(ControlMode::PercentOutput, 1);
	highTransport.Set(ControlMode::PercentOutput, 1);

}, {}));

conveyorToggle.WhenReleased(frc2::InstantCommand([&] {
	
	lowTransport.Set(ControlMode::PercentOutput, 0);
	highTransport.Set(ControlMode::PercentOutput, 0);

}, {}));


}





}//namespace