//includes
#include "commands/EncoderDrive.h"
#include "Robot.h"
#include "subsystems/DriveTrain.h"
#include "Util.h"

#include <math.h> //for math.abs
#include <frc/drive/Vector2d.h>  //for mechanum calculations
//end includes

namespace ohs2020 {

//EncoderDrive

//constructors
EncoderDrive::EncoderDrive(int x, int y, double a){
	m_X = x*HORIZONTAL_CALIBRATION;
	m_Y = y;
	m_A = a;
} //base constructor

EncoderDrive::EncoderDrive(double x, double y, double a) : EncoderDrive(static_cast <int> (x*CPI), static_cast <int> (y*CPI), 0.0) {} 
//constructor for inches w/ angle

EncoderDrive::EncoderDrive(int x, int y) : EncoderDrive(x, y, 0) {}
//constructor for tick movement w/o angle

EncoderDrive::EncoderDrive(double x, double y) : EncoderDrive(x, y, 0.0) {}
//constructor for inch movement w/o angle

//end constructors

//override commands
void EncoderDrive::Initialize() {
		
	DebugOutF("CPI: "+ std::to_string(CPI));
	DebugOutF("Encoder Init Pos: "+ std::to_string( Robot::Get().GetDriveTrain().GetLFront()->GetSensorCollection().GetIntegratedSensorPosition() ) );
	
	//wheel calculations
	frc::Vector2d input{EncoderDrive::GetX() , EncoderDrive::GetY()};
	//input.Rotate(EncoderDrive::GetA()); NOTE: moved turn to end of command 
	//calculates relative wheel speeds

	//reduce number to -1-1
	double divisor = static_cast <double>(abs(input.x)+abs(input.y));
	double percentOut = (static_cast <double>(input.y) + static_cast <double>(input.x) ) / divisor;
	//divisor is used to convert encoder ticks to numbers from -1-1

	//sets wheel speeds
	Robot::Get().GetDriveTrain().GetLFront()->Set(ControlMode::PercentOutput, 1);
	Robot::Get().GetDriveTrain().GetRFront()->Set(ControlMode::PercentOutput, (static_cast <double>(input.y) - static_cast <double>(input.x) ) / divisor);
	Robot::Get().GetDriveTrain().GetLBack()->Set(ControlMode::PercentOutput, (static_cast <double>(input.y) - static_cast <double>(input.x) ) / divisor);
	Robot::Get().GetDriveTrain().GetRBack()->Set(ControlMode::PercentOutput, (static_cast <double>(input.y) + static_cast <double>(input.x) ) / divisor);
	//end set wheel speeds
	
	DebugOutF("Calculations: " + std::to_string(percentOut));
	DebugOutF("Calc2: " + std::to_string(input.y));
	DebugOutF("Calc2.5: " + std::to_string(input.x));
	DebugOutF("Calc3: " + std::to_string(divisor));
	//

	EncoderDrive::SetX( 
		Robot::Get().GetDriveTrain().GetLFront()->GetSensorCollection().GetIntegratedSensorPosition() +
		static_cast <int> ( (EncoderDrive::GetX()+EncoderDrive::GetY()) * percentOut )
		);//modifies x by relative movement of fl wheel and adds it to current quadrature pos to find target quadrature pos

	//uses top left wheel for encoder tracking
	
	DebugOutF("Target Encoder Pos: "+ std::to_string(EncoderDrive::GetX()));

}//starts motor turn

bool EncoderDrive::IsFinished() {
	return abs( 
			abs(Robot::Get().GetDriveTrain().GetLFront()->GetSensorCollection().GetIntegratedSensorPosition()) 
			- abs(EncoderDrive::GetX())  
			) < COUNT_THRESHOLD;

}//returns true when encoderTicks is equals to or greater than target

void EncoderDrive::Execute() {DebugOutF(std::to_string(Robot::Get().GetDriveTrain().GetLFront()->GetMotorOutputPercent()));}//execute command (does nothing, waits)

void EncoderDrive::End(bool interrupted) {

	double dir = static_cast <double> (EncoderDrive::GetA()) / abs(EncoderDrive::GetA()) * 0.1;

	DebugOutF("|STOP|");
	
	Robot::Get().GetDriveTrain().GetLFront()->Set(dir);
	Robot::Get().GetDriveTrain().GetRFront()->Set(-dir);
	Robot::Get().GetDriveTrain().GetLBack()->Set(dir);
	Robot::Get().GetDriveTrain().GetRBack()->Set(-dir);
	
	while( abs( abs( Robot::Get().GetNavX()->GetYaw()) - abs(EncoderDrive::GetA()) ) > 2){

	}//waits for navx to return data

	//stop motors
	Robot::Get().GetDriveTrain().GetLFront()->Set(0);
	Robot::Get().GetDriveTrain().GetRFront()->Set(0);
	Robot::Get().GetDriveTrain().GetLBack()->Set(0);
	Robot::Get().GetDriveTrain().GetRBack()->Set(0);
}//stops motors and exits
//end override commands

//end EncoderDrive

}//end namespace