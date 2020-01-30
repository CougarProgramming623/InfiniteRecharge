/*----------------------------------------------------------------------------*/
/* Copyright (c) 2017-2019 FIRST. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#include "Robot.h"

#include <frc/smartdashboard/SmartDashboard.h>
#include <frc2/command/CommandScheduler.h>
#include <frc2/command/PrintCommand.h>

#include "Cob.h"

namespace ohs2020 {

	Robot* Robot::s_Instance = nullptr;

Robot::Robot() {

	s_Instance = this;

}

void Robot::RobotInit() {
	Cob::Init();
	m_DriveTrain.Init();
    m_oi.Init();



	try{
		navx = new AHRS(SPI::Port::kMXP);
	} catch (std::exception &ex){
		std::string err = "Error instantiating navX MXP: ";
		err += ex.what();
		DebugOutF(err.c_str());
		
	}
	frc2::CommandScheduler::GetInstance().Schedule(new frc2::PrintCommand("Hello"));
	navx->ZeroYaw();
}

/**
 * This function is called every robot packet, no matter the mode. Use
 * this for items like diagnostics that you want to run during disabled,
 * autonomous, teleoperated and test.
 *
 * <p> This runs after the mode specific periodic functions, but before
 * LiveWindow and SmartDashboard integrated updating.
 */
void Robot::RobotPeriodic() {

	frc2::CommandScheduler::GetInstance().Run();

	DebugOutF("ID 1: " + std::to_string(m_oi.GetButtonBoard().GetRawButton(1)));
	DebugOutF("ID 2: " + std::to_string(m_oi.GetButtonBoard().GetRawButton(2)));
	DebugOutF("ID 3: " + std::to_string(m_oi.GetButtonBoard().GetRawButton(3)));
	DebugOutF("ID 4: " + std::to_string(m_oi.GetButtonBoard().GetRawButton(4)));
	DebugOutF("ID 5: " + std::to_string(m_oi.GetButtonBoard().GetRawButton(5)));
	DebugOutF("ID 6: " + std::to_string(m_oi.GetButtonBoard().GetRawButton(6)));
	DebugOutF("ID 6: " + std::to_string(m_oi.GetButtonBoard().GetRawButton(7)));	

	/*
	if (m_oi.isFodToggle()) {
		DebugOutF("Fod = true");
	} else {
		DebugOutF("Fod = false");
	}
	*/

	Cob::PushValue(CobKey::ROTATION, navx->GetYaw());
	Cob::PushValue(CobKey::TIME_LEFT, frc2::Timer::GetMatchTime().to<double>());

    //Cob::PushValue(CobKey::MODE, isFodMode());
	//DebugOutF("FOD: " + std::to_string(GetOI().IsFOD()));
}

/**
 * This function is called once each time the robot enters Disabled mode. You
 * can use it to reset any subsystem information you want to clear when the
 * robot is disabled.
 */
void Robot::DisabledInit() {

	
}

void Robot::DisabledPeriodic() {


}

/**
 * This autonomous runs the autonomous command selected by your {@link
 * RobotContainer} class.
 */
void Robot::AutonomousInit() {
	navx->ZeroYaw();
}

void Robot::AutonomousPeriodic() {

}

void Robot::TeleopInit() {
	// This makes sure that the autonomous stops running when
	// teleop starts running. If you want the autonomous to
	// continue until interrupted by another command, remove
	// this line or comment it out.
	if (m_autonomousCommand != nullptr) {
		m_autonomousCommand->Cancel();
		m_autonomousCommand = nullptr;
	}
}

/**
 * This function is called periodically during operator control.
 */
void Robot::TeleopPeriodic() {


}

/**
 * This function is called periodically during test mode.
 */
void Robot::TestPeriodic() {


}


}//namespace


int main() {
	return frc::StartRobot<ohs2020::Robot>();
}
