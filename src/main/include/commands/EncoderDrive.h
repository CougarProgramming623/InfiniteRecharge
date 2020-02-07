#pragma once

//includes
#include <frc2/command/Command.h>
#include <math.h>
//end includes

namespace ohs2020 {

//robot information variables
const unsigned short int COUNTS_PER_ROTATION = 2048; //encoder counts per rotation, falcon500 integrated = 2048CPR
const float WHEEL_DIAMETER = 8.0f; //diameter in inches
const double GEAR_RATIO = 1.0/1.0; //gear ratio of the bot

	//calculated variables
const double CPI = (COUNTS_PER_ROTATION * GEAR_RATIO) / (WHEEL_DIAMETER * (atan(1)*4)/*PI*/ ); //counter per rotation of this wheel
const double HORIZONTAL_CALIBRATION = 1/1; //intended/actual, because mechanim wheels skid a variable to change how many ticks are needed for horizontal traverse
	//end calculated variables

//end robot information variables

class EncoderDrive : public frc2::Command {
	
public:
	//constructors
	EncoderDrive(int x, int y);//using ticks
	EncoderDrive(double x, double y);//using inches
	//end constructors

	//overrides
	void Initialize() override;
	void Execute() override;
	bool IsFinished() override;
	void End(bool interrupted) override;
	//end of overrides
private:

	//movement variables
	double m_x, m_y;//x and y traverse
	//end movement variables

};//end class

}//end namespace