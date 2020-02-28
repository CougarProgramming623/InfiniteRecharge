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
lowConveyor(RobotID::GetID(LOW_TRANSPORT)),
highConveyor(RobotID::GetID(HIGH_TRANSPORT)),
FlyWheelEncoder(RobotID::GetID(FLYWHEEL)),

launcher( [&] 		{ return Robot::Get().GetOI().GetButtonBoard().GetRawButton(2); 	}),
flyWheelToggle([&] 	{ return Robot::Get().GetOI().GetButtonBoard().GetRawButton(4);	}), 
conveyorToggle( [&] { return Robot::Get().GetOI().GetButtonBoard().GetRawButton(15); 	}),
timer() {

	RemoveRegistry(this, &Flywheel, &feeder, &lowConveyor, &highConveyor);

}

void Shooter::Init() {

	SetupShooterButtons();
	SetupConveyorButtons();

}

void Shooter::SetupShooterButtons() {

	flyWheelToggle.WhenPressed(frc2::FunctionalCommand([this]{}, [this] { //on execute

		isFlywheelOn = true;
		flywheelWU = (int)((double)Flywheel.GetSelectedSensorVelocity() / 2048 * 600);
		// DebugOutF(std::to_string(flywheelWU));
		frc::SmartDashboard::PutNumber("Flywheel Speed", flywheelWU);

        double speed =  3000 + 500 * Robot::Get().GetOI().GetButtonBoard().GetRawAxis(0);
		Flywheel.Set(ControlMode::Velocity, speed);
		DebugOutF("speed: " + std::to_string(speed));

	}, [this] (bool f){//on end
		DebugOutF("shooter end");

		isFlywheelOn = false;

		Flywheel.Set(ControlMode::Velocity, 0);

	}, [this] { return !flyWheelToggle.Get(); }, {}));

	flyWheelToggle.WhenReleased(frc2::RunCommand([&] {

		flywheelWU = (int)((double)Flywheel.GetSelectedSensorVelocity() / 2048 * 600);
		frc::SmartDashboard::PutNumber("Flywheel Speed", flywheelWU);

		// DebugOutF(std::to_string(flywheelWU));

	}, {}));


	launcher.WhenHeld(frc2::RunCommand([&] {

		feeder.Set(ControlMode::PercentOutput, 1);

	}, {} ));

	launcher.WhenReleased(frc2::InstantCommand([&] {

		feeder.Set(ControlMode::PercentOutput, 0);

	}, {} ));
} 


void Shooter::SetupConveyorButtons() {

conveyorToggle.WhenHeld(frc2::RunCommand([&] {

	lowConveyor.Set(ControlMode::PercentOutput, .5);
	highConveyor.Set(ControlMode::PercentOutput, 1);

}, {}));

conveyorToggle.WhenReleased(frc2::InstantCommand([&] {
	
	lowConveyor.Set(ControlMode::PercentOutput, 0);
	highConveyor.Set(ControlMode::PercentOutput, 0);

}, {}));

}

void Shooter::ReverseConveyor() {
	frc2::CommandScheduler::GetInstance().Schedule(new frc2::SequentialCommandGroup(

		frc2::InstantCommand( [&] { 
			highConveyor.Set(ControlMode::PercentOutput, -1);
		}, {}), 

		frc2::WaitCommand(units::second_t(1)),

		frc2::InstantCommand( [&] {
			highConveyor.Set(ControlMode::PercentOutput, 0);
		}, {})

	));
}

frc2::SequentialCommandGroup* Shooter::Shoot() {
	frc2::SequentialCommandGroup* group = new frc2::SequentialCommandGroup();

	frc2::InstantCommand startFlywheel = frc2::InstantCommand( [&] {
		Flywheel.Set(ControlMode::Velocity, 16000);
	}, {});

	frc2::InstantCommand startFeeder = frc2::InstantCommand( [&] {
		feeder.Set(ControlMode::PercentOutput, 1);
	}, {});

	frc2::InstantCommand stopShoot = frc2::InstantCommand( [&] {
		Flywheel.Set(ControlMode::Velocity, 0);
		feeder.Set(ControlMode::PercentOutput, 0);
	}, {});

	frc2::InstantCommand spinConveyers = frc2::InstantCommand( [&] { 
		lowConveyor.Set(ControlMode::PercentOutput, 0.5);
		highConveyor.Set(ControlMode::PercentOutput, 1.0); 
	}, {});

	frc2::InstantCommand stopConveyers = frc2::InstantCommand( [&] { 
		lowConveyor.Set(ControlMode::PercentOutput, 0.0);
		highConveyor.Set(ControlMode::PercentOutput, 0.0);
	}, {});

	group->AddCommands(startFlywheel, frc2::WaitCommand(units::second_t(1.0)), startFeeder, spinConveyers, frc2::WaitCommand(units::second_t(3.0)), stopConveyers, stopShoot);

	return group;
}



}//namespace