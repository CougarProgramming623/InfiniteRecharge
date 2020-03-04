#pragma once

//includes
#include <frc2/command/CommandBase.h>
#include <frc2/command/CommandHelper.h>
#include <wpi/SmallSet.h>
//end includes 

namespace ohs2020 {

class EncoderDriveM : public frc2::CommandHelper<frc2::CommandBase, EncoderDriveM>{
	
public:

	//constructors
	EncoderDriveM(int x, int y, int a);//passes # of encoder ticks travelled
	EncoderDriveM(double x, double y, int a);//pass in distance in inches and converts to # of encoder ticks
	EncoderDriveM(int x, int y);//passes # of encoder ticks travelled without a rotation value
	EncoderDriveM(double x, double y);//pass in distance in inches and converts to # of encoder ticks without a rotation value
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

	//movement variables
	int m_X, m_Y, m_A;
	//m_X:# of ticks to move left and right, negative being left, positive being right
	//m_Y:# of ticks to move forward and back, negative being reverse, positive being forward
	//m_A:# of ticks to turn, negative being left, and positive being right (relative to 'forward' on wheels)

	int m_InitialTicks [4];
	//initial position values for all 4 wheels for tracking relative progress while running MotionMagicPID

	//end movement variables

};//end class

}//end namespace

//(!) WARN (!) change var names and method names to UPPER camel case via F2