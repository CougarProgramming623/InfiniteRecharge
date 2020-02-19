#include "subsystems/Shooter.h"
#include "Robot.h"
#include "ohs/Log.h"

#include <frc/smartdashboard/SmartDashboard.h>

#include "frc2/command/FunctionalCommand.h"

namespace ohs2020{

const double DefaultShooterPower = 1;

Shooter::Shooter() : 

m_Flywheel(35),
m_Feeder(3),
m_Launcher( [&] { return Robot::Get().GetOI().GetButtonBoard().GetRawButton(6); }), // Arm Override
m_FlyWheelToggle([&] { return Robot::Get().GetOI().GetButtonBoard().GetRawButton(1); }), //Vacuum Toggle Switch
m_FlyWheelEncoder(35),
m_Timer()
{}

void Shooter::Init() {

	SetupShooterButtons();
}

inline void Shooter::SetupShooterButtons() {

	m_FlyWheelToggle.WhileHeld(frc2::FunctionalCommand([this]{}, [this] { //on execute

		m_IsFlywheelOn = true;
		m_FlywheelWU = m_Flywheel.GetSelectedSensorVelocity() / 4;
		frc::SmartDashboard::PutNumber("Flywheel Speed", m_FlywheelWU);

		m_Flywheel.Set(ControlMode::PercentOutput, Robot::Get().GetOI().GetButtonBoard().GetRawAxis(0));

	}, [this] (bool f){//on end

		m_IsFlywheelOn = false;

		m_Flywheel.Set(ControlMode::PercentOutput, 0);

	}, [this] { return false; }, {}));

	std::vector<std::unique_ptr<frc2::Command>> vector;
	frc2::FunctionalCommand* shootBall = new frc2::FunctionalCommand([this] { //on init

		m_Timer.Reset();
		m_Timer.Start();
		m_Feeder.Set(ControlMode::PercentOutput, 1);
		OHS_DEBUG([](auto& f){ f << "shooting init"; });

	}, [this] {}, [this] (bool f) {// on end

		m_Feeder.Set(ControlMode::PercentOutput, 0);
		OHS_DEBUG([](auto& f){ f << "shooting end"; });


	}, [this] { // is finished
		
		OHS_DEBUG([&](auto& f){ f << "shooter is finished? " << (m_Timer.Get() > units::second_t(1)); })
		return m_Timer.Get() > units::second_t(2);


	}, {});
	for(int i = 0; i < 100; i++){
		vector.push_back(std::unique_ptr<frc2::Command>(shootBall));
		vector.push_back(std::make_unique<frc2::WaitCommand>(units::second_t(1)));
	}
	m_Launcher.WhenHeld(frc2::SequentialCommandGroup(std::move(vector)));

}
}//namespace