#include "subsystems/Intake.h"
#include "Robot.h"
#include "ohs/RobotID.h"

#include <frc2/command/InstantCommand.h>
#include <frc2/command/FunctionalCommand.h>

using namespace ohs623;

namespace ohs2020{

Intake::Intake() :
intakeMotor(RobotID::GetID(INTAKE)),
intakeLift(RobotID::GetID(INTAKE_LIFTER)),

intakeDownButton([&] 	{ return Robot::Get().GetOI().GetButtonBoard().GetRawButton(9); }), // >
intakeUpButton([&]		{ return Robot::Get().GetOI().GetButtonBoard().GetRawButton(10); }), //  Arm/Wrist Dial
intakeManualButton([&]	{ return Robot::Get().GetOI().GetButtonBoard().GetRawButton(11); }), // >

intakeOnButton([&] 		{ return Robot::Get().GetOI().GetButtonBoard().GetRawButton(7); }), //Fork Override Yellow Button
timer()
{}

void Intake::Init() {

IntakeToggleCommands();
IntakePlacementCommands();

}

void Intake::IntakeToggleCommands() {
	intakeOnButton.WhenPressed(frc2::InstantCommand([&]{ intakeOn = true; DebugOutF("On"); }, {} ));
	intakeOnButton.WhenReleased(frc2::InstantCommand([&]{ intakeOn = false; DebugOutF("Off"); }, {} ));
	
}

void Intake::IntakePlacementCommands() {
	
IntakePlacementUp();
IntakePlacementDown();
ManualModeCommands();

}

void Intake::IntakePlacementUp() {

	intakeUpButton.WhenPressed(frc2::FunctionalCommand([this] { //on init

		intakeLift.Set(ControlMode::PercentOutput, .5);
		timer.Reset();
		timer.Start();

		DebugOutF("Lifting");

	}, [this] {}, [this] (bool f) { //on end

		intakeLift.Set(ControlMode::PercentOutput, 0); 

		DebugOutF("Done");

	}, [this]{//is finished

		return timer.Get() > units::second_t(1);

	}, {} ));

}

void Intake::IntakePlacementDown() {

	intakeDownButton.WhenPressed(frc2::FunctionalCommand([this] { //on init

		intakeLift.Set(ControlMode::PercentOutput, -.5);
		timer.Reset();
		timer.Start();

		DebugOutF("Dropping");

	}, [this] {}, [this] (bool f) { //on end

		intakeLift.Set(ControlMode::PercentOutput, 0); 

		DebugOutF("Done");

	}, [this]{//is finished

		return timer.Get() > units::second_t(1);

	}, {} ));
}

void Intake::ManualModeCommands() {

	intakeManualButton.WhenPressed(frc2::InstantCommand([&] { DebugOutF("Manual Mode"); }, {}));

	intakeManualButton.WhenHeld(frc2::RunCommand([&] { 

		intakeLift.Set(ControlMode::PercentOutput, Robot::Get().GetOI().GetButtonBoard().GetRawAxis(1));

	}, {}));
}

}//namespace