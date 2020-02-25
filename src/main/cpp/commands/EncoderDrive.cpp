//includes
#include "commands/EncoderDrive.h"
#include "Robot.h"
#include "subsystems/DriveTrain.h"
#include "Util.h"

#include <memory>
#include <frc/drive/Vector2d.h>  //for mechanum calculations
#include <wpi/ArrayRef.h>
#include <frc/smartdashboard/SmartDashboard.h>

//end includes

namespace ohs2020 {

//EncoderDrive

//constructors
EncoderDrive::EncoderDrive(int x, int y, int a){
	m_X = x*HORIZONTAL_CALIBRATION;
	m_Y = y;
	m_A = a;
	AddRequirements(wpi::ArrayRef<frc2::Subsystem*>(&Robot::Get().GetDriveTrain()));
} //base constructor

EncoderDrive::EncoderDrive(double x, double y, int a) : EncoderDrive(static_cast <int> (x*CPI), static_cast <int> (y*CPI), 0) {} 
//constructor for inches w/ angle

EncoderDrive::EncoderDrive(int x, int y) : EncoderDrive(x, y, 0) {}
//constructor for tick movement w/o angle

EncoderDrive::EncoderDrive(double x, double y) : EncoderDrive(x, y, 0) {}
//constructor for inch movement w/o angle

//end constructors

//override commands
void EncoderDrive::Initialize() {

	m_InitialTicks[0] = Robot::Get().GetDriveTrain().GetLFront()->GetSelectedSensorPosition();
	m_InitialTicks[1] = Robot::Get().GetDriveTrain().GetRFront()->GetSelectedSensorPosition();
	m_InitialTicks[2] = Robot::Get().GetDriveTrain().GetLBack()->GetSelectedSensorPosition();
	m_InitialTicks[3] = Robot::Get().GetDriveTrain().GetRBack()->GetSelectedSensorPosition();

	Robot::Get().GetDriveTrain().SetBrakeMode(false);

	Robot::Get().GetDriveTrain().UsePositionPID();

	enableJoySticks = false;

	DebugOutF("ERROR:FINISH ENCODER DRIVE INIT");
	
}//starts motor turn

bool EncoderDrive::IsFinished() {
	
	return
		ctre::phoenix::Utilities::IsWithin( Robot::Get().GetDriveTrain().GetLFront()->GetSelectedSensorPosition(), (m_InitialTicks[0] + m_Y + m_X + m_A), COUNT_THRESHOLD ) &&
		ctre::phoenix::Utilities::IsWithin( Robot::Get().GetDriveTrain().GetRFront()->GetSelectedSensorPosition(), (m_InitialTicks[1] + m_Y - m_X - m_A), COUNT_THRESHOLD ) &&
		ctre::phoenix::Utilities::IsWithin( Robot::Get().GetDriveTrain().GetLBack()->GetSelectedSensorPosition(), (m_InitialTicks[2] + m_Y - m_X + m_A), COUNT_THRESHOLD ) &&
		ctre::phoenix::Utilities::IsWithin( Robot::Get().GetDriveTrain().GetRBack()->GetSelectedSensorPosition(), (m_InitialTicks[3] + m_Y + m_X - m_A), COUNT_THRESHOLD );

}//returns true when encoderTicks is equals to or greater than target

void EncoderDrive::Execute() {
	DebugOutF("ERROR:MOVING");
	Robot::Get().GetDriveTrain().GetLFront()->Set(ControlMode::Position, m_InitialTicks[0]+(m_Y + m_X + m_A) );
	Robot::Get().GetDriveTrain().GetRFront()->Set(ControlMode::Position, m_InitialTicks[1]+(m_Y - m_X - m_A) );
	Robot::Get().GetDriveTrain().GetLBack()->Set(ControlMode::Position,  m_InitialTicks[2]+(m_Y - m_X + m_A) );
	Robot::Get().GetDriveTrain().GetRBack()->Set(ControlMode::Position,  m_InitialTicks[3]+(m_Y + m_X - m_A) );

}//execute command

void EncoderDrive::End(bool interrupted) {
	
	DebugOutF("ERROR:|STOP|");
	DebugOutF("ERROR:Final Pos diff: "+std::to_string( Robot::Get().GetDriveTrain().GetLFront()->GetSelectedSensorPosition() - (m_Y + m_A + m_InitialTicks[0] + m_X) ));

	Robot::Get().GetDriveTrain().SetBrakeMode(true);
	Robot::Get().GetDriveTrain().UseVelocityPID();

	Robot::Get().GetDriveTrain().GetLFront()->Set(ControlMode::PercentOutput, 0 );
	Robot::Get().GetDriveTrain().GetRFront()->Set(ControlMode::PercentOutput, 0 );
	Robot::Get().GetDriveTrain().GetLBack()->Set(ControlMode::PercentOutput,  0 );
	Robot::Get().GetDriveTrain().GetRBack()->Set(ControlMode::PercentOutput,  0 );

	enableJoySticks = true;
}//stops motors and exits
//end override commands

//end EncoderDrive

}//end namespace