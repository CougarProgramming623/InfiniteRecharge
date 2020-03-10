#include "subsystems/Shooter.h"
#include "Robot.h"
#include "ohs/RobotID.h"
#include "ohs/Log.h"
#include "Util.h"
#include "commands/TurnToPosSlow.h"

#include <frc/smartdashboard/SmartDashboard.h>
#include <frc/smartdashboard/SendableRegistry.h>

#include "frc2/command/FunctionalCommand.h"

using namespace ohs623;

namespace ohs2020{

const double DefaultShooterPower = 1;

Shooter::Shooter() : 

m_Flywheel(RobotID::GetID(FLYWHEEL)),
m_Feeder(RobotID::GetID(FEEDER)),
m_LowConveyor(RobotID::GetID(LOW_TRANSPORT)),
m_HighConveyor(RobotID::GetID(HIGH_TRANSPORT)),
m_FlyWheelEncoder(RobotID::GetID(FLYWHEEL)),

m_Launcher([&] 		{ return Robot::Get().GetOI().GetButtonBoard().GetRawButton(2); 	}),
m_FlyWheelToggle([&] 	{ return Robot::Get().GetOI().GetButtonBoard().GetRawButton(4); 	}), 
m_ConveyorToggle([&] 	{ return Robot::Get().GetOI().GetButtonBoard().GetRawButton(15); 	}),
m_ReverseFeeder([&]	{ return Robot::Get().GetOI().GetDriverJoystick().GetRawButton(2); 	}),
m_TurnShoot([&] 	{ return Robot::Get().GetOI().GetButtonBoard().GetRawButton(3); 	}),

m_BloopFeeder([&]  { return m_HighConveyor.IsFwdLimitSwitchClosed(); }),

m_Timer() {

	RemoveRegistry(this, &m_Flywheel, &m_Feeder, &m_LowConveyor, &m_HighConveyor);

}

void Shooter::Init() {

	SetupShooterButtons();
	SetupConveyorButtons();
	volt = std::array<double, 10>();
	index = 0;

	for(unsigned int i = 0; i < volt.size(); i++){
		volt[i] = 0.0;
	}//set vals for volt

}

void Shooter::SetupShooterButtons() {

	m_FlyWheelToggle.WhenPressed(frc2::FunctionalCommand([this]{}, [this] { //on execute

		m_IsFlywheelOn = true;
		m_FlywheelWU = (int)((double)m_Flywheel.GetSelectedSensorVelocity() / 2048 * 600);
		// DebugOutF(std::to_string(m_FlywheelWU));
		frc::SmartDashboard::PutNumber("Flywheel Speed", m_FlywheelWU);

    double speed = 3000 + 1000 * Robot::Get().GetOI().GetButtonBoard().GetRawAxis(0);
    Robot::Get().GetOI().GetButtonBoard().SetOutput(2, m_FlywheelWU > 4400);
		
		m_Flywheel.Set(ControlMode::Velocity, speed);
		Cob::PushValue(CobKey::FLYWHEEL_SPEED, speed );

	}, [this] (bool f){//on end
		DebugOutF("shooter end");

		m_IsFlywheelOn = false;

		m_Flywheel.Set(ControlMode::Velocity, 0);

	}, [this] { return !m_FlyWheelToggle.Get(); }, {}));

	m_FlyWheelToggle.WhenReleased(frc2::RunCommand([&] {

		m_FlywheelWU = (int)((double)m_Flywheel.GetSelectedSensorVelocity() / 2048 * 600);
		frc::SmartDashboard::PutNumber("Flywheel Speed", m_FlywheelWU);
        Robot::Get().GetOI().GetButtonBoard().SetOutput(2, false);

		// DebugOutF(std::to_string(m_FlywheelWU));

	}, {}));


	m_Launcher.WhenHeld(frc2::RunCommand([&] {

		m_Feeder.Set(ControlMode::PercentOutput, 1);

	}, {} ));

	m_Launcher.WhenReleased(frc2::InstantCommand([&] {

		m_Feeder.Set(ControlMode::PercentOutput, 0);

	}, {} ));

	m_ReverseFeeder.WhenHeld(frc2::RunCommand([&] {

		m_HighConveyor.Set(ControlMode::PercentOutput, -1);
		m_LowConveyor.Set(ControlMode::PercentOutput, -1);

	}, {} ));

	m_ReverseFeeder.WhenReleased(frc2::InstantCommand([&] {

		m_HighConveyor.Set(ControlMode::PercentOutput, 0);
		m_LowConveyor.Set(ControlMode::PercentOutput, 0);

	}, {} ));

	frc2::SequentialCommandGroup* shootandturn = new frc2::SequentialCommandGroup();

	shootandturn->AddCommands(TurnToPosSlow());
	shootandturn->AddCommands(frc2::WaitCommand(units::second_t(3)));
	shootandturn->AddCommands(frc2::InstantCommand([&] {

		m_Flywheel.Set(ControlMode::PercentOutput, 1);
		m_Feeder.Set(ControlMode::PercentOutput, 1);

		m_LowConveyor.Set(ControlMode::PercentOutput, .5);
		m_HighConveyor.Set(ControlMode::PercentOutput, 1);

	}, {}));

	m_TurnShoot.WhenPressed(shootandturn);

	m_TurnShoot.WhenReleased(frc2::InstantCommand([&] {

		m_Flywheel.Set(ControlMode::PercentOutput, 0);
		m_Feeder.Set(ControlMode::PercentOutput, 0);

		m_LowConveyor.Set(ControlMode::PercentOutput, 0);
		m_HighConveyor.Set(ControlMode::PercentOutput, 0);


	}, {}));
}

void Shooter::SetupConveyorButtons() {

m_ConveyorToggle.WhenHeld(frc2::RunCommand([&] {

	m_LowConveyor.Set(ControlMode::PercentOutput, .5);
	m_HighConveyor.Set(ControlMode::PercentOutput, 1);
	
	Robot::Get().GetOI().GetButtonBoard().SetOutput(1, m_HighConveyor.IsFwdLimitSwitchClosed());


}, {}));

m_ConveyorToggle.WhenReleased(frc2::InstantCommand([&] {
	
	m_LowConveyor.Set(ControlMode::PercentOutput, 0);
	m_HighConveyor.Set(ControlMode::PercentOutput, 0);
	Robot::Get().GetOI().GetButtonBoard().SetOutput(1, false);

}, {}));

m_BloopFeeder.WhenPressed(frc2::SequentialCommandGroup(

frc2::InstantCommand([&] { m_Feeder.Set(ControlMode::PercentOutput, 1); }, {}),
frc2::WaitCommand(units::second_t(.25)),
frc2::InstantCommand([&] { m_Feeder.Set(ControlMode::PercentOutput, 0); }, {})

));

}

void Shooter::ReverseConveyor() {
	frc2::CommandScheduler::GetInstance().Schedule(new frc2::SequentialCommandGroup(

		frc2::InstantCommand( [&] { 
			m_HighConveyor.Set(ControlMode::PercentOutput, -1);
		}, {}), 

		frc2::WaitCommand(units::second_t(1)),

		frc2::InstantCommand( [&] {
			m_HighConveyor.Set(ControlMode::PercentOutput, 0);
		}, {})

	));
}

frc2::SequentialCommandGroup Shooter::Shoot(double wait) {
	frc2::SequentialCommandGroup group = frc2::SequentialCommandGroup();

	frc2::InstantCommand startFlywheel = frc2::InstantCommand( [&] {
		m_Flywheel.Set(ControlMode::Velocity, 3145 / 600 * 2048);
	}, {});

	frc2::InstantCommand startFeeder = frc2::InstantCommand( [&] {
		m_Feeder.Set(ControlMode::PercentOutput, 1);
	}, {});

	frc2::InstantCommand stopShoot = frc2::InstantCommand( [&] {
		m_Flywheel.Set(ControlMode::Velocity, 0);
		m_Feeder.Set(ControlMode::PercentOutput, 0);
	}, {});

	frc2::InstantCommand spinConveyers = frc2::InstantCommand( [&] { 
		m_LowConveyor.Set(ControlMode::PercentOutput, 0.5);
		m_HighConveyor.Set(ControlMode::PercentOutput, 1.0); 
	}, {});

	frc2::InstantCommand stopConveyers = frc2::InstantCommand( [&] { 
		m_LowConveyor.Set(ControlMode::PercentOutput, 0.0);
		m_HighConveyor.Set(ControlMode::PercentOutput, 0.0);
	}, {});

	group.AddCommands(startFlywheel, frc2::WaitCommand(units::second_t(1.0)), startFeeder, spinConveyers, frc2::WaitCommand(units::second_t(wait)), stopConveyers, stopShoot);

	return group;
}

bool Shooter::CheckVolt(){//(!)(!)(!) NOT VOLTAGE IS AMPS
	volt[index % volt.size()] = m_HighConveyor.GetStatorCurrent();
	index++;

	if(volt[volt.size()] > 0.0){
	
		double avg  = 0;

		for(unsigned int i = 0; i < volt.size(); i++){
			avg += volt[i];
		}

		avg /= volt.size();

		if(avg >= 1.0){

			ReverseConveyor();

			return true;
		}

	}

	return false;

}

}//namespace