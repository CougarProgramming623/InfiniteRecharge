//includes
#include "commands/EncoderDriveV.h"
#include "commands/EncoderDriveP.h"
#include "Robot.h"

#include <math.h> //for math.abs
#include <memory>
//end includes

namespace ohs2020 {

//EncoderDriveV

//constructors
EncoderDriveV::EncoderDriveV(int x, int y, int a, double max){
	m_X = static_cast <int> (x*HORIZONTAL_CALIBRATION);
	m_Y = static_cast <int> (y*VERTICAL_CALIBRATION);
	m_A = a;
	m_Max = max;
	AddRequirements(&Robot::Get().GetDriveTrain());//prevents other commands that require drivetrain from running (!)DOES NOT WORK(!)
} //base constructor

EncoderDriveV::EncoderDriveV(double x, double y, int a, double max) : EncoderDriveV(static_cast <int> (x*CPI), static_cast <int> (y*CPI), a, max) {} 
//constructor for inches w/ angle

EncoderDriveV::EncoderDriveV(int x, int y, double max) : EncoderDriveV(x, y, 0, max) {}
//constructor for tick movement w/o angle

EncoderDriveV::EncoderDriveV(double x, double y, double max) : EncoderDriveV(x, y, 0, max) {}
//constructor for inch movement w/o angle

//end constructors

//override commands
void EncoderDriveV::Initialize() {

	m_InitialTicks = Robot::Get().GetDriveTrain().GetLFront()->GetSelectedSensorPosition();//grabs front left sensor position for objective tracking

	Robot::Get().GetDriveTrain().SetBrakeMode(false);//brakeMode may cause drive to stutter

}//starts motor turn

bool EncoderDriveV::IsFinished() {

	if(!frc::DriverStation::GetInstance().IsAutonomous()) return true;//ensure encoderDrive does not interfere with joystick

	if( m_X + m_Y + m_A < 0 ){
		return Robot::Get().GetDriveTrain().GetLFront()->GetSelectedSensorPosition() <= (m_Y + m_A + m_InitialTicks + m_X);
	} else {
		return Robot::Get().GetDriveTrain().GetLFront()->GetSelectedSensorPosition() >= (m_Y + m_A + m_InitialTicks + m_X);
	}//checks if encoderdrive has reached or overshot the target angle

}//returns true when encoderTicks is equals to or greater than target

void EncoderDriveV::Execute() {
	
	int max = abs(m_X)+abs(m_A)+abs(m_Y);//used to convert a speed to a number between -1 and 1 to be multiplied by maxSpeed so target velcity are always within max speed
	double maxSpeed = m_Max*(6380/60/10*2048);//% of allowable max speed to run the bot at

	//sets motor velocities
	Robot::Get().GetDriveTrain().GetLFront()->Set(ControlMode::Velocity, (m_Y + m_X + m_A)/max*maxSpeed );
	Robot::Get().GetDriveTrain().GetRFront()->Set(ControlMode::Velocity, (m_Y - m_X - m_A)/max*maxSpeed );
	Robot::Get().GetDriveTrain().GetLBack()->Set(ControlMode::Velocity, (m_Y - m_X + m_A)/max*maxSpeed );
	Robot::Get().GetDriveTrain().GetRBack()->Set(ControlMode::Velocity, (m_Y + m_X - m_A)/max*maxSpeed );
	//end set motor velocities

}//execute command (does nothing, waits)

void EncoderDriveV::End(bool interrupted) {
	Robot::Get().GetDriveTrain().SetBrakeMode(true);//turns brakemode back on for DriveWithJoystick

	//ensures wheels are stopped
	Robot::Get().GetDriveTrain().GetLFront()->Set(ControlMode::PercentOutput, 0 );
	Robot::Get().GetDriveTrain().GetRFront()->Set(ControlMode::PercentOutput, 0 );
	Robot::Get().GetDriveTrain().GetLBack()->Set(ControlMode::PercentOutput,  0 );
	Robot::Get().GetDriveTrain().GetRBack()->Set(ControlMode::PercentOutput,  0 );

}//stops motors and exits
//end override commands

//end EncoderDriveV

}//end namespace