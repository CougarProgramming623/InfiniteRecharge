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
#include <frc/DriverStation.h>
#include "Cob.h"
#include "AutoManager.h"

#include "ohs/RobotID.h"
#include "ohs/Log.h"



namespace ohs2020 {

	Robot* Robot::s_Instance = nullptr;

Robot::Robot() {

	s_Instance = this;

}


void Robot::RobotInit() {

	m_AutoManager.AutoInit();
	Cob::Init();
	m_DriveTrain.Init();
	m_oi.Init();
	m_shooter.Init();
	m_climb.Init();
	Intake* intake = new Intake();
	intake->Init();

	RemoveRegistry(navx);

	try {
		navx = new AHRS(SPI::Port::kMXP);
	} catch (std::exception &ex){
		std::string err = "Error instantiating navX MXP: ";
		err += ex.what();
		DebugOutF(err.c_str());
		
	}
	navx->ZeroYaw();

	m_Init = true;


	m_led.SetLength(kLength);

	for(int i = 0; i < kLength; i++){
		m_ledBuffer[i].SetRGB(0, 255, 0);
	}

	m_led.SetData(m_ledBuffer);
	m_led.Start();


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

	Cob::InMesUpdate();

	frc2::CommandScheduler::GetInstance().Run();

	Cob::PushValue(CobKey::IN_USE_AUTO, m_AutoManager.GetInUse());
	Cob::PushValue(CobKey::ROTATION, navx->GetYaw());
	Cob::PushValue(CobKey::FLYWHEEL_WU, m_shooter.GetFlywheelWU());
	Cob::PushValue(CobKey::FLYWHEEL_STATUS, m_shooter.GetFlywheelState());
	Cob::PushValue(CobKey::TIME_LEFT, frc2::Timer::GetMatchTime().to<double>());
	Cob::PushValue(CobKey::CURRENT_DELAY, m_AutoManager.GetDelay());
	Cob::PushValue(CobKey::COB_CHECK, m_CobCheck);
	m_CobCheck++;
	
	if(frc::DriverStation::GetInstance().GetAlliance() == frc::DriverStation::Alliance::kRed){
		Cob::PushValue(CobKey::IS_RED, true);
	}else{
		Cob::PushValue(CobKey::IS_RED, false);
	}
	
	if (frc::DriverStation::GetInstance().IsDisabled()){
		Cob::PushValue(CobKey::MODE, 5);
		//DebugOutF("set to 5");
	}else if (frc::DriverStation::GetInstance().IsAutonomous()){
		Cob::PushValue(CobKey::MODE, 2);
		//DebugOutF("set to 2");
	}else if (m_oi.IsFOD()){
		Cob::PushValue(CobKey::MODE, 0);
		//DebugOutF("set to 0");
	}else {
		Cob::PushValue(CobKey::MODE, 1);

	}

	if(frc::DriverStation::GetInstance().GetAlliance() == frc::DriverStation::Alliance::kRed){
		for(int i = 0; i < kLength; i++){
			m_ledBuffer[i].SetRGB(255, 0, 0);
		}
	} else if (frc::DriverStation::GetInstance().GetAlliance() == frc::DriverStation::Alliance::kBlue){
		for(int i = 0; i < kLength; i++){
			m_ledBuffer[i].SetRGB(0, 0, 255);
		}
	} else {
		for(int i = 0; i < kLength; i++){
			m_ledBuffer[i].SetRGB(0, 255, 0);
		}
	}
	
	m_led.SetData(m_ledBuffer);

	//DebugOutF("FOD: " + std::to_string(GetOI().IsFOD()));
}

/**
 * This function is called once each time the robot enters Disabled mode. You
 * can use it to reset any subsystem information you want to clear when the
 * robot is disabled.
 */

void Robot::DisabledInit() {

	m_climb.isDeployed = false;
	m_climb.isDeployFinished = false;

	m_DriveTrain.SetBrakeMode(false);
}

void Robot::DisabledPeriodic() {


}

/**
 * This autonomous runs the autonomous command selected by your {@link
 * RobotContainer} class.
 */
void Robot::AutonomousInit() {
	navx->ZeroYaw();
	m_autonomousCommand = m_AutoManager.GetAuto();
	frc2::CommandScheduler::GetInstance().Schedule(m_autonomousCommand);
	m_DriveTrain.SetBrakeMode(true);
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
	m_DriveTrain.SetBrakeMode(true);
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

bool CanAssertionsQuit() {
	return true;//Maybe disable during competitions
}