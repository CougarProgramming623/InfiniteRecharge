//includes
#include "commands/EncoderDrive.h"
#include "Robot.h"
#include "subsystems/DriveTrain.h"
#include "Util.h"

#include <math.h> //for math.abs
#include <frc/drive/Vector2d.h>  //for mechanum calculations
#include <wpi/ArrayRef.h>
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
	m_InitialTicks = Robot::Get().GetDriveTrain().GetLFront()->GetSelectedSensorPosition();
	Robot::Get().GetDriveTrain().GetLFront()->SetNeutralMode(ctre::phoenix::motorcontrol::NeutralMode::Coast);
	Robot::Get().GetDriveTrain().GetRFront()->SetNeutralMode(ctre::phoenix::motorcontrol::NeutralMode::Coast);
	Robot::Get().GetDriveTrain().GetLBack()->SetNeutralMode(ctre::phoenix::motorcontrol::NeutralMode::Coast);
	Robot::Get().GetDriveTrain().GetRBack()->SetNeutralMode(ctre::phoenix::motorcontrol::NeutralMode::Coast);
}//starts motor turn

bool EncoderDrive::IsFinished() {
	DebugOutF("DIFF:"+ std::to_string(m_Y+m_A+m_InitialTicks+m_X)+ "|"+std::to_string(Robot::Get().GetDriveTrain().GetLFront()->GetSelectedSensorPosition()) + "|" + std::to_string(abs((m_InitialTicks+m_X+m_Y+m_A) - Robot::Get().GetDriveTrain().GetLFront()->GetSelectedSensorPosition()) <= COUNT_THRESHOLD));
	DebugOutF("MOTOR%: "+ std::to_string(Robot::Get().GetDriveTrain().GetLFront()->GetMotorOutputPercent()));

	if( m_X + m_Y + m_A < 0 ){
		return Robot::Get().GetDriveTrain().GetLFront()->GetSelectedSensorPosition() <= (m_Y+m_A+m_InitialTicks+m_X);
	}else{
		return Robot::Get().GetDriveTrain().GetLFront()->GetSelectedSensorPosition() >= (m_Y+m_A+m_InitialTicks+m_X);
	}
	/*abs( 
			abs(Robot::Get().GetDriveTrain().GetLFront()->GetSensorCollection().GetIntegratedSensorPosition()) 
			- abs(EncoderDrive::GetX())  
			) < COUNT_THRESHOLD;*/

}//returns true when encoderTicks is equals to or greater than target

void EncoderDrive::Execute() {

	int max = 1;
	double maxSpeed = 0.4;

	if(m_X > m_Y){
		max = m_X+m_A;
	}else{
		max = m_Y+m_A;
	}

	Robot::Get().GetDriveTrain().GetLFront()->Set(ControlMode::PercentOutput, (m_Y+m_X+m_A)/max*maxSpeed );
	Robot::Get().GetDriveTrain().GetRFront()->Set(ControlMode::PercentOutput, (m_Y-m_X-m_A)/max*maxSpeed );
	Robot::Get().GetDriveTrain().GetLBack()->Set(ControlMode::PercentOutput, (m_Y-m_X+m_A)/max*maxSpeed );
	Robot::Get().GetDriveTrain().GetRBack()->Set(ControlMode::PercentOutput, (m_Y+m_X-m_A)/max*maxSpeed );
}//execute command (does nothing, waits)

void EncoderDrive::End(bool interrupted) {
	
	DebugOutF("|STOP|"+std::to_string(interrupted));
	DebugOutF("Final Pos: "+std::to_string(Robot::Get().GetDriveTrain().GetLFront()->GetSelectedSensorPosition()));

	Robot::Get().GetDriveTrain().GetLFront()->SetNeutralMode(ctre::phoenix::motorcontrol::NeutralMode::Brake);
	Robot::Get().GetDriveTrain().GetRFront()->SetNeutralMode(ctre::phoenix::motorcontrol::NeutralMode::Brake);
	Robot::Get().GetDriveTrain().GetLBack()->SetNeutralMode(ctre::phoenix::motorcontrol::NeutralMode::Brake);
	Robot::Get().GetDriveTrain().GetRBack()->SetNeutralMode(ctre::phoenix::motorcontrol::NeutralMode::Brake);

}//stops motors and exits
//end override commands

//end EncoderDrive

}//end namespace