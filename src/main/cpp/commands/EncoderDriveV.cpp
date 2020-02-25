//includes
#include "commands/EncoderDriveV.h"
#include "commands/EncoderDrive.h"
#include "commands/DriveWithJoysticks.h"
#include "Robot.h"
#include "subsystems/DriveTrain.h"
#include "Util.h"

#include <math.h> //for math.abs
#include <memory>
#include <frc/drive/Vector2d.h>  //for mechanum calculations
#include <wpi/ArrayRef.h>
//end includes

namespace ohs2020 {

//EncoderDriveV

//constructors
EncoderDriveV::EncoderDriveV(int x, int y, int a){
	m_X = x*HORIZONTAL_CALIBRATION;
	m_Y = y*VERTICAL_CALIBRATION;
	m_A = a;
	AddRequirements(&Robot::Get().GetDriveTrain());
} //base constructor

EncoderDriveV::EncoderDriveV(double x, double y, int a) : EncoderDriveV(static_cast <int> (x*CPI*(12/13)), static_cast <int> (y*CPI*(12/13)), 0) {} 
//constructor for inches w/ angle

EncoderDriveV::EncoderDriveV(int x, int y) : EncoderDriveV(x, y, 0) {}
//constructor for tick movement w/o angle

EncoderDriveV::EncoderDriveV(double x, double y) : EncoderDriveV(x, y, 0) {}
//constructor for inch movement w/o angle

//end constructors

//override commands
void EncoderDriveV::Initialize() {

	m_InitialTicks = Robot::Get().GetDriveTrain().GetLFront()->GetSelectedSensorPosition();

	Robot::Get().GetDriveTrain().GetLFront()->SetNeutralMode(ctre::phoenix::motorcontrol::NeutralMode::Coast);
	Robot::Get().GetDriveTrain().GetRFront()->SetNeutralMode(ctre::phoenix::motorcontrol::NeutralMode::Coast);
	Robot::Get().GetDriveTrain().GetLBack()->SetNeutralMode(ctre::phoenix::motorcontrol::NeutralMode::Coast);
	Robot::Get().GetDriveTrain().GetRBack()->SetNeutralMode(ctre::phoenix::motorcontrol::NeutralMode::Coast);

}//starts motor turn

bool EncoderDriveV::IsFinished() {

	DebugOutF("DIFF:"+ std::to_string(m_Y + m_A + m_InitialTicks + m_X - Robot::Get().GetDriveTrain().GetLFront()->GetSelectedSensorPosition()));
	DebugOutF("MOTOR%: "+ std::to_string(Robot::Get().GetDriveTrain().GetLFront()->GetMotorOutputPercent()));

	if( m_X + m_Y + m_A < 0 ){
		return Robot::Get().GetDriveTrain().GetLFront()->GetSelectedSensorPosition() <= (m_Y + m_A + m_InitialTicks + m_X);
	} else {
		return Robot::Get().GetDriveTrain().GetLFront()->GetSelectedSensorPosition() >= (m_Y + m_A + m_InitialTicks + m_X);
	}

}//returns true when encoderTicks is equals to or greater than target

void EncoderDriveV::Execute() {
	

	int max = abs(m_X)+abs(m_A)+abs(m_Y);
	double maxSpeed = 0.4;

	Robot::Get().GetDriveTrain().GetLFront()->Set(ControlMode::Velocity, (m_Y + m_X + m_A)/max*maxSpeed*DriveTrain::kMAX_VELOCITY );
	Robot::Get().GetDriveTrain().GetRFront()->Set(ControlMode::Velocity, (m_Y - m_X - m_A)/max*maxSpeed*DriveTrain::kMAX_VELOCITY );
	Robot::Get().GetDriveTrain().GetLBack()->Set(ControlMode::Velocity, (m_Y - m_X + m_A)/max*maxSpeed*DriveTrain::kMAX_VELOCITY );
	Robot::Get().GetDriveTrain().GetRBack()->Set(ControlMode::Velocity, (m_Y + m_X - m_A)/max*maxSpeed*DriveTrain::kMAX_VELOCITY);

}//execute command (does nothing, waits)

void EncoderDriveV::End(bool interrupted) {
	
	DebugOutF("|STOP|"+std::to_string(interrupted));
	DebugOutF("ERROR:Final Pos diff: "+std::to_string( Robot::Get().GetDriveTrain().GetLFront()->GetSelectedSensorPosition() - (m_Y + m_A + m_InitialTicks + m_X) ));

	Robot::Get().GetDriveTrain().SetBrakeMode(true);

	Robot::Get().GetDriveTrain().GetLFront()->Set(ControlMode::PercentOutput, 0 );
	Robot::Get().GetDriveTrain().GetRFront()->Set(ControlMode::PercentOutput, 0 );
	Robot::Get().GetDriveTrain().GetLBack()->Set(ControlMode::PercentOutput,  0 );
	Robot::Get().GetDriveTrain().GetRBack()->Set(ControlMode::PercentOutput,  0 );
}//stops motors and exits
//end override commands

//end EncoderDriveV

}//end namespace