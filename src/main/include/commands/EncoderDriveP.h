#pragma once

//includes
#include <frc2/command/CommandBase.h>
#include <frc2/command/CommandHelper.h>
#include <math.h>
#include <wpi/SmallSet.h>
//end includes 

namespace ohs2020 {

//drivetrain information variables
const unsigned short int COUNTS_PER_ROTATION = 2048; //encoder counts per rotation, falcon500 integrated = 2048CPR
const float WHEEL_DIAMETER = 8.0f; //diameter in inches
const double GEAR_RATIO = 12.0/1.0; //gear ratio of the bot

	//calculated variables
const double CPI = (COUNTS_PER_ROTATION * GEAR_RATIO) / ((WHEEL_DIAMETER * (atan(1)*4) )/*PI*/ ); //counter per rotation of this wheel
	//end calculated variables

const int COUNT_THRESHOLD = 100;//accuracy threshold of counts

//end drivetrain information variables

class EncoderDriveP : public frc2::CommandHelper<frc2::CommandBase, EncoderDriveP>{
	
public:

	//constructors
	EncoderDriveP(int x, int y, int a);//passes # of encoder ticks travelled
	EncoderDriveP(double x, double y, int a);//pass in distance in inches and converts to # of encoder ticks
	EncoderDriveP(int x, int y);//passes # of encoder ticks travelled without a rotation value
	EncoderDriveP(double x, double y);//pass in distance in inches and converts to # of encoder ticks without a rotation value
	//end constructors

	//overrides
	void Initialize() override;
	void Execute() override;
	bool IsFinished() override;
	void End(bool interrupted) override;
	virtual wpi::SmallSet<frc2::Subsystem*, 4> GetRequirements() const override{
		return wpi::SmallSet<frc2::Subsystem*, 4> (); 
	}
	
	virtual std::unique_ptr<frc2::Command> TransferOwnership() && override{
		return std::unique_ptr<frc2::Command>(this);
	}
	//end of overrides

private:

	//wheel calibration numbers due to mechanim wheel skidding or calculation inaccuracies
	const double HORIZONTAL_CALIBRATION = 36.0/31.0; 
	const double VERTICAL_CALIBRATION = 36.0/43.0;
	/*current known calibration values for 'saber'
	Vertical Traverse [position: 36.0/31.5 | Velocity: 36.0/43.0 | Magic: ?/?]
	Horizontal Traverse [position: ?/? | Velocity: 36.0/31.0 | Magic: ?/?]
	Calculation: (intended/actual) */

	//movement variables
	int m_X, m_Y, m_A;
	//m_X:# of ticks to move left and right, negative being left, positive being right
	//m_Y:# of ticks to move forward and back, negative being reverse, positive being forward
	//m_A:# of ticks to turn, negative being left, and positive being right (relative to 'forward' on wheels)

	int m_InitialTicks [4];
	//initial position values for all 4 wheels for tracking relative progress while running PositionPID

	//end movement variables

};//end class

}//end namespace

//(!) WARN (!) change var names and method names to UPPER camel case via F2