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

	m_FlyWheelToggle.WhileHeld(frc2::FunctionalCommand([this]{}, [this] { //on execute

		DebugOutF(std::to_string(flywheelWU));
		flywheelWU = (int)((double)Flywheel.GetSelectedSensorVelocity() / 2048 * 600);
		m_FlywheelWU = m_Flywheel.GetSelectedSensorVelocity() / 4;

		m_Flywheel.Set(ControlMode::PercentOutput, Robot::Get().GetOI().GetButtonBoard().GetRawAxis(0));

	}, [this] (bool f){//on end

		m_IsFlywheelOn = false;

		m_Flywheel.Set(ControlMode::PercentOutput, 0);

	}, [this] { return false; }, {}));

	flyWheelToggle.WhenReleased(frc2::RunCommand([&] {

		flywheelWU = (int)((double)Flywheel.GetSelectedSensorVelocity() / 2048 * 600);
		DebugOutF(std::to_string(flywheelWU));

	}, {}));

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

}//namespace