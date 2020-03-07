//includes
#include "commands/EncoderDriveP.h"
#include "Robot.h"

#include <memory>

//end includes

namespace ohs2020 {

//EncoderDriveP

//constructors
EncoderDriveP::EncoderDriveP(int x, int y, int a){
	m_X = x*HORIZONTAL_CALIBRATION;
	m_Y = y*VERTICAL_CALIBRATION;
	m_A = a;
	AddRequirements(&Robot::Get().GetDriveTrain());//prevents other commands that require drivetrain from running (!)DOES NOT WORK(!)
} //base constructor

EncoderDriveP::EncoderDriveP(double x, double y, int a) : EncoderDriveP(static_cast <int> (x*CPI), static_cast <int> (y*CPI), 0) {} 
//constructor for inches w/ angle

EncoderDriveP::EncoderDriveP(int x, int y) : EncoderDriveP(x, y, 0) {}
//constructor for tick movement w/o angle

EncoderDriveP::EncoderDriveP(double x, double y) : EncoderDriveP(x, y, 0) {}
//constructor for inch movement w/o angle

//end constructors

//override commands
void EncoderDriveP::Initialize() {

	//limits max wheel speed for PID	
	Robot::Get().GetDriveTrain().GetLFront()->ConfigPeakOutputForward(0.5,0);
	Robot::Get().GetDriveTrain().GetRFront()->ConfigPeakOutputForward(0.5,0);
	Robot::Get().GetDriveTrain().GetLBack()->ConfigPeakOutputForward(0.5,0);
	Robot::Get().GetDriveTrain().GetRBack()->ConfigPeakOutputForward(0.5,0);

	Robot::Get().GetDriveTrain().GetLFront()->ConfigPeakOutputReverse(-0.5,0);
	Robot::Get().GetDriveTrain().GetRFront()->ConfigPeakOutputReverse(-0.5,0);
	Robot::Get().GetDriveTrain().GetLBack()->ConfigPeakOutputReverse(-0.5,0);
	Robot::Get().GetDriveTrain().GetRBack()->ConfigPeakOutputReverse(-0.5,0);
	//

	//ensures motor is not moving when starting	
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

	Robot::Get().GetDriveTrain().SetBrakeMode(false);//brakemode may cause stuttering

	Robot::Get().GetDriveTrain().UsePositionPID();//toggles PID values for motors to use Position Drive Mode

}//starts motor turn

bool EncoderDriveP::IsFinished() {
	
	return
		abs( Robot::Get().GetDriveTrain().GetLFront()->GetSelectedSensorPosition() - (m_InitialTicks[0] + m_Y + m_X + m_A)) <= COUNT_THRESHOLD &&
		abs( Robot::Get().GetDriveTrain().GetRFront()->GetSelectedSensorPosition() - (m_InitialTicks[1] + m_Y - m_X - m_A)) <= COUNT_THRESHOLD &&
		abs( Robot::Get().GetDriveTrain().GetLBack()->GetSelectedSensorPosition() - (m_InitialTicks[2] + m_Y - m_X + m_A)) <= COUNT_THRESHOLD &&
		abs( Robot::Get().GetDriveTrain().GetRBack()->GetSelectedSensorPosition() - (m_InitialTicks[3] + m_Y + m_X - m_A)) <= COUNT_THRESHOLD;
	//returns true if all values are within accuracy threshold (defined in EncoderDriveP.h)

}//returns true when encoderTicks is equals to or greater than target

void EncoderDriveP::Execute() {
	
	Robot::Get().GetDriveTrain().UsePositionPID();

	//sets motor targets for PID to traverse to
	Robot::Get().GetDriveTrain().GetLFront()->Set(ControlMode::Position, m_InitialTicks[0]+(m_Y + m_X + m_A) );
	Robot::Get().GetDriveTrain().GetRFront()->Set(ControlMode::Position, m_InitialTicks[1]+(m_Y - m_X - m_A) );
	Robot::Get().GetDriveTrain().GetLBack()->Set(ControlMode::Position,  m_InitialTicks[2]+(m_Y - m_X + m_A) );
	Robot::Get().GetDriveTrain().GetRBack()->Set(ControlMode::Position,  m_InitialTicks[3]+(m_Y + m_X - m_A) );
	//

}//execute command

void EncoderDriveP::End(bool interrupted) {
	
	Robot::Get().GetDriveTrain().SetBrakeMode(true);
	Robot::Get().GetDriveTrain().UseVelocityPID();//resets PID mode as Velcity is default PID mode

	//ensures wheels are stopped
	Robot::Get().GetDriveTrain().GetLFront()->Set(ControlMode::PercentOutput, 0 );
	Robot::Get().GetDriveTrain().GetRFront()->Set(ControlMode::PercentOutput, 0 );
	Robot::Get().GetDriveTrain().GetLBack()->Set(ControlMode::PercentOutput,  0 );
	Robot::Get().GetDriveTrain().GetRBack()->Set(ControlMode::PercentOutput,  0 );
	//

	//reset max motor output to full
	Robot::Get().GetDriveTrain().GetLFront()->ConfigPeakOutputForward(1,0);
	Robot::Get().GetDriveTrain().GetRFront()->ConfigPeakOutputForward(1,0);
	Robot::Get().GetDriveTrain().GetLBack()->ConfigPeakOutputForward(1,0);
	Robot::Get().GetDriveTrain().GetRBack()->ConfigPeakOutputForward(1,0);

	Robot::Get().GetDriveTrain().GetLFront()->ConfigPeakOutputReverse(-1,0);
	Robot::Get().GetDriveTrain().GetRFront()->ConfigPeakOutputReverse(-1,0);
	Robot::Get().GetDriveTrain().GetLBack()->ConfigPeakOutputReverse(-1,0);
	Robot::Get().GetDriveTrain().GetRBack()->ConfigPeakOutputReverse(-1,0);
	//

}//stops motors and exits
//end override commands

//end EncoderDriveP

}//end namespace