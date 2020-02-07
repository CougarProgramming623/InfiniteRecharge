//includes
#include "commands/EncoderDrive.h"
#include "Robot.h"
#include "subsystems/DriveTrain.h"
//end includes

namespace ohs2020 {

//EncoderDrive

//constructors
EncoderDrive::EncoderDrive(int x, int y) {
	m_x = x;
	m_y = y;
}//constructor for tick movement

EncoderDrive::EncoderDrive(double x, double y) : EncoderDrive(static_cast <int> (x*CPI), static_cast <int> (y*CPI))
{}//constructor for inch movement
//end constructors

//override commands
void Initialize() {}//start motor turn

bool IsFinished() {
	
}//returns true when encoderTicks is equals to or greater than target

void Execute() {}//execute command

void End(bool interrupted) {
	Robot::Get().GetDriveTrain().GetLFront().Set(0);
	Robot::Get().GetDriveTrain().GetRFront().Set(0);
	Robot::Get().GetDriveTrain().GetLBack().Set(0);
	Robot::Get().GetDriveTrain().GetRBack().Set(0);
}//stops motors and exits
//end override commands

//end EncoderDrive

}//end namespace