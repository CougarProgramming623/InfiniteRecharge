#include "subsystems/Shooter.h"
#include "Robot.h"
#include "ohs/RobotID.h"
#include "ohs/Log.h"

#include <frc/smartdashboard/SmartDashboard.h>

#include "frc2/command/FunctionalCommand.h"

using namespace ohs623;

namespace ohs2020 {

const double DefaultShooterPower = 1;

Shooter::Shooter() : 

m_Flywheel(RobotID::GetID(FLYWHEEL)),
m_Feeder(RobotID::GetID(FEEDER)),
m_Launcher(OHS_BUTTON(LAUNCHER_ID)), // Arm Override
m_FlyWheelToggle(OHS_BUTTON(FLYWHEEL_TOGGLE_ID)), //Vacuum Toggle Switch
m_FlyWheelEncoder(RobotID::GetID(FLYWHEEL)),
m_Timer() {

	//Flywheel.ConfigAllowableClosedloopError
	m_Flywheel.Config_kF(0, .25, 0);

}

void Shooter::Init() {

	SetupShooterButtons();
}

inline void Shooter::SetupShooterButtons() {

	m_FlyWheelToggle.WhileHeld(frc2::FunctionalCommand([this]{}, [this] { //on execute

		m_IsFlywheelOn = true;
		m_FlywheelWU = m_Flywheel.GetSelectedSensorVelocity();
		DebugOutF(std::to_string(m_FlywheelWU));
		frc::SmartDashboard::PutNumber("Flywheel Speed", m_FlywheelWU);

		m_Flywheel.Set(ControlMode::Velocity, Robot::Get().GetOI().GetButtonBoard().GetRawAxis(0) * 22000);

	}, [this] (bool f){//on end

		m_IsFlywheelOn = false;

		m_Flywheel.Set(ControlMode::Velocity, 0);

	}, [this] { return false; }, {}));

	m_FlyWheelToggle.WhenReleased(frc2::RunCommand([&] {

		m_FlywheelWU = m_Flywheel.GetSelectedSensorVelocity();
		DebugOutF(std::to_string(m_FlywheelWU));

	}, {}));

}

}//namespace