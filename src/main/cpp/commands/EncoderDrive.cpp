//includes
#include "commands/EncoderDrive.h"
#include "Robot.h"
#include "subsystems/DriveTrain.h"

#include <math.h> //for math.abs
#include <frc/drive/Vector2d.h>  //for mechanum calculations
//end includes

namespace ohs2020 {

//EncoderDrive

//constructors
EncoderDrive::EncoderDrive(int x, int y, double a){
	m_x = x*HORIZONTAL_CALIBRATION;
	m_y = y;
	m_a = a;
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

	//wheel calculations
	frc::Vector2d input{EncoderDrive::getX() , EncoderDrive::getY()};
	input.Rotate(EncoderDrive::getA()); 
	//calculates relative wheel speeds

	//reduce number to -1-1
	int divisor = abs(input.x)+abs(input.y)+abs(EncoderDrive::getA());
	//divisor is used to convert encoder ticks to numbers from -1-1

	//sets wheel speeds
	Robot::Get().GetDriveTrain().GetLFront().Set(ControlMode::PercentOutput, (static_cast <double>(input.y) + input.x + EncoderDrive::getA()) / divisor );
	Robot::Get().GetDriveTrain().GetRFront().Set(ControlMode::PercentOutput, (static_cast <double>(input.y) - input.x - EncoderDrive::getA()) / divisor );
	Robot::Get().GetDriveTrain().GetLBack().Set(ControlMode::PercentOutput, (static_cast <double>(input.y) - input.x + EncoderDrive::getA()) / divisor );
	Robot::Get().GetDriveTrain().GetRBack().Set(ControlMode::PercentOutput, (static_cast <double>(input.y) + input.x - EncoderDrive::getA()) / divisor );
	//end set wheel speeds

	//

	EncoderDrive::setX( 
		Robot::Get().GetDriveTrain().GetLFront().GetSensorCollection().GetQuadraturePosition() +
		static_cast <int> ( EncoderDrive::getX() * Robot::Get().GetDriveTrain().GetLFront().GetMotorOutputPercent() )
		);//modifies x by relative movement of fl wheel and adds it to current quadrature pos to find target quadrature pos

	//uses top left wheel for encoder tracking

}//starts motor turn

bool EncoderDrive::IsFinished() {
	return abs( 
			abs(Robot::Get().GetDriveTrain().GetLFront().GetSensorCollection().GetQuadraturePosition()) 
			- abs(EncoderDrive::getX())  
			) < COUNT_THRESHOLD;

}//returns true when encoderTicks is equals to or greater than target

void EncoderDrive::Execute() {}//execute command (does nothing, waits)

void EncoderDrive::End(bool interrupted) {
	Robot::Get().GetDriveTrain().GetLFront().Set(0);
	Robot::Get().GetDriveTrain().GetRFront().Set(0);
	Robot::Get().GetDriveTrain().GetLBack().Set(0);
	Robot::Get().GetDriveTrain().GetRBack().Set(0);
}//stops motors and exits
//end override commands

//end EncoderDrive

}//end namespace