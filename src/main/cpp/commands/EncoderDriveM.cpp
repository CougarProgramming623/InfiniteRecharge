//includes
#include "commands/EncoderDriveM.h"
#include "commands/EncoderDriveP.h"
#include "Robot.h"

#include <memory>

//end includes

namespace ohs2020 {

//EncoderDriveM

//constructors	
EncoderDriveM::EncoderDriveM(int x, int y, int a){
	m_X = x*HORIZONTAL_CALIBRATION;
	m_Y = y;
	m_A = a;
	AddRequirements(&Robot::Get().GetDriveTrain());//prevents other commands that require drivetrain from running (!)DOES NOT WORK(!)
} //base constructor

EncoderDriveM::EncoderDriveM(double x, double y, int a) : EncoderDriveM(static_cast <int> (x*CPI), static_cast <int> (y*CPI), 0) {} 
//constructor for inches w/ angle

EncoderDriveM::EncoderDriveM(int x, int y) : EncoderDriveM(x, y, 0) {}
//constructor for tick movement w/o angle

EncoderDriveM::EncoderDriveM(double x, double y) : EncoderDriveM(x, y, 0) {}
//constructor for inch movement w/o angle

//end constructors

//override commands
void EncoderDriveM::Initialize() {

	//ensures motors are not moving when starting
	Robot::Get().GetDriveTrain().GetLFront()->Set(ControlMode::PercentOutput, 0 );
	Robot::Get().GetDriveTrain().GetRFront()->Set(ControlMode::PercentOutput, 0 );
	Robot::Get().GetDriveTrain().GetLBack()->Set(ControlMode::PercentOutput,  0 );
	Robot::Get().GetDriveTrain().GetRBack()->Set(ControlMode::PercentOutput,  0 );
	//

	//gets all sensor position for error checking
	m_InitialTicks[0] = Robot::Get().GetDriveTrain().GetLFront()->GetSelectedSensorPosition();
	m_InitialTicks[1] = Robot::Get().GetDriveTrain().GetRFront()->GetSelectedSensorPosition();
	m_InitialTicks[2] = Robot::Get().GetDriveTrain().GetLBack()->GetSelectedSensorPosition();
	m_InitialTicks[3] = Robot::Get().GetDriveTrain().GetRBack()->GetSelectedSensorPosition();
	//

	Robot::Get().GetDriveTrain().SetBrakeMode(false);

	Robot::Get().GetDriveTrain().UseMagicPID();//toggles PID values for motors to use Motion Magic Drive Mode

}//starts motor turn

bool EncoderDriveM::IsFinished() {
	
	return
		abs( Robot::Get().GetDriveTrain().GetLFront()->GetSelectedSensorPosition() - (m_InitialTicks[0] + m_Y + m_X + m_A)) <= COUNT_THRESHOLD &&
		abs( Robot::Get().GetDriveTrain().GetRFront()->GetSelectedSensorPosition() - (m_InitialTicks[1] + m_Y - m_X - m_A)) <= COUNT_THRESHOLD &&
		abs( Robot::Get().GetDriveTrain().GetLBack()->GetSelectedSensorPosition() - (m_InitialTicks[2] + m_Y - m_X + m_A)) <= COUNT_THRESHOLD &&
		abs( Robot::Get().GetDriveTrain().GetRBack()->GetSelectedSensorPosition() - (m_InitialTicks[3] + m_Y + m_X - m_A)) <= COUNT_THRESHOLD;
	//returns true if all values are within accuracy threshold (defined in EncoderDriveP.h)

}//returns true when encoderTicks is equals to or greater than target

void EncoderDriveM::Execute() {
	
	Robot::Get().GetDriveTrain().UseMagicPID();

	//sets motor targets for PID to traverse to
	Robot::Get().GetDriveTrain().GetLFront()->Set(ControlMode::MotionMagic, m_InitialTicks[0]+(m_Y + m_X + m_A) );
	Robot::Get().GetDriveTrain().GetRFront()->Set(ControlMode::MotionMagic, m_InitialTicks[1]+(m_Y - m_X - m_A) );
	Robot::Get().GetDriveTrain().GetLBack()->Set(ControlMode::MotionMagic,  m_InitialTicks[2]+(m_Y - m_X + m_A) );
	Robot::Get().GetDriveTrain().GetRBack()->Set(ControlMode::MotionMagic,  m_InitialTicks[3]+(m_Y + m_X - m_A) );
	//

}//execute command

void EncoderDriveM::End(bool interrupted) {
	
	Robot::Get().GetDriveTrain().SetBrakeMode(true);
	Robot::Get().GetDriveTrain().UseVelocityPID();//resets PID mode as Velcity is default PID mode

	//ensures wheels are stopped
	Robot::Get().GetDriveTrain().GetLFront()->Set(ControlMode::PercentOutput, 0 );
	Robot::Get().GetDriveTrain().GetRFront()->Set(ControlMode::PercentOutput, 0 );
	Robot::Get().GetDriveTrain().GetLBack()->Set(ControlMode::PercentOutput,  0 );
	Robot::Get().GetDriveTrain().GetRBack()->Set(ControlMode::PercentOutput,  0 );
	//

}//stops motors and exits
//end override commands

//end EncoderDriveM

}//end namespace