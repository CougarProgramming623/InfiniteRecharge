	#include "subsystems/Intake.h"
#include "Robot.h"
#include "ohs/RobotID.h"

#include <frc2/command/InstantCommand.h>
#include <frc2/command/FunctionalCommand.h>
#include <frc2/command/StartEndCommand.h>


using namespace ohs623;

namespace ohs2020{

Intake::Intake() :
intakeMotor(RobotID::GetID(INTAKE)),
intakeMotorDos(RobotID::GetID(INTAKE_DOS)),

intakeLift(RobotID::GetID(INTAKE_LIFTER)),

intakeDownButton([&] 	{ return ButtonID INTAKE_DOWN_ID; }), // >
intakeUpButton([&]		{ return ButtonID INTAKE_UP_ID; }), //  Arm/Wrist Dial
intakeManualButton([&]	{ return ButtonID INTAKE_MANUAL_ID; }), // >

intakeOnButton([&] 		{ return ButtonID INTAKE_TOGGLE_ID; }), //Fork Override Yellow Button
timer()
{}

void Intake::Init() {

	IntakeToggleCommands();
	//IntakePlacementCommands();
	intakeMotorDos.Follow(intakeMotor);

}

void Intake::IntakeToggleCommands() {
	// intakeOnButton.WhenPressed(frc2::InstantCommand([&]{ intakeOn = true; DebugOutF("On"); }, {} ));
	// intakeOnButton.WhenReleased(frc2::InstantCommand([&]{ intakeOn = false; DebugOutF("Off"); }, {} ));
	intakeOnButton.WhenPressed(frc2::StartEndCommand(
		// Start driving forward at the start of the command
		[this] { intakeMotor.Set(1.0); DebugOutF("intake on"); },
		// Stop driving at the end of the command
		[this] { intakeMotor.Set(0.0); },
		// Requires the drive subsystem
		{}
	));
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