#pragma once

//includes
#include <frc2/command/CommandBase.h>
#include <frc2/command/CommandHelper.h>
#include <wpi/SmallSet.h>
//end includes 

namespace ohs2020 {

class EncoderDriveV : public frc2::CommandHelper<frc2::CommandBase, EncoderDriveV>{
	
public:

	//constructors
	EncoderDriveV(int x, int y, int a, double max);//passes # of encoder ticks travelled
	EncoderDriveV(double x, double y, int a, double max);//pass in distance in inches and converts to # of encoder ticks
	EncoderDriveV(int x, int y, double max);//passes # of encoder ticks travelled without a rotation value
	EncoderDriveV(double x, double y, double max);//pass in distance in inches and converts to # of encoder ticks without a rotation value
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
	
	int m_InitialTicks;
	//the starting number of ticks of the 'front left' wheel, uses this number to track relative progress in driving
	
	double m_Max;
	//maximum speed to run motors at

	//end movement variables

};//end class

}//end namespace

//(!) WARN (!) change var names and method names to UPPER camel case via F2