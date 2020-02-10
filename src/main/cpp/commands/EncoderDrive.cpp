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
EncoderDrive::EncoderDrive(int x, int y, double a){
	m_X = x*HORIZONTAL_CALIBRATION;
	m_Y = y;
	m_A = a;
	AddRequirements(wpi::ArrayRef<frc2::Subsystem*>(&Robot::Get().GetDriveTrain()));

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
	m_InitialTicks = Robot::Get().GetDriveTrain().GetLFront()->GetSelectedSensorPosition();
}//starts motor turn

bool EncoderDrive::IsFinished() {
	return Robot::Get().GetDriveTrain().GetLFront()->GetSelectedSensorPosition() > GetY() + m_InitialTicks; 
	/*abs( 
			abs(Robot::Get().GetDriveTrain().GetLFront()->GetSensorCollection().GetIntegratedSensorPosition()) 
			- abs(EncoderDrive::GetX())  
			) < COUNT_THRESHOLD;*/

}//returns true when encoderTicks is equals to or greater than target

void EncoderDrive::Execute() {
	
	Robot::Get().GetDriveTrain().CartesianDrive(0.3, 0.0, 0.0, 0.0);

	DebugOutF("Current Encoder Pos: "+ std::to_string(Robot::Get().GetDriveTrain().GetLFront()->GetSelectedSensorPosition()));

}//execute command (does nothing, waits)

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