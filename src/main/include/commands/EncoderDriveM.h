#pragma once

//includes
#include <frc2/command/CommandBase.h>
#include <frc2/command/CommandHelper.h>
#include <frc2/command/SequentialCommandGroup.h>
#include <math.h>
#include <wpi/SmallSet.h>
//end includes 

namespace ohs2020 {

class EncoderDriveM : public frc2::CommandHelper<frc2::CommandBase, EncoderDriveM> /*frc2::CommandHelper<frc2::CommandBase, EncoderDrive> */{
	
public:
	//constructors
	EncoderDriveM(int x, int y, int a);//using ticks
	EncoderDriveM(double x, double y, int a);//using inches
	EncoderDriveM(int x, int y);//ticks w/o rot
	EncoderDriveM(double x, double y);//inches w/o rot
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
	int m_X, m_Y, m_A;//x and y traverse (horizontal(x) & vertical(y)) in encoder ticks, EncoderTurnTicks(A)
	int m_InitialTicks [4];
	//end movement variables

};//end class

}//end namespace

//(!) WARN (!) change var names and method names to UPPER camel case via F2