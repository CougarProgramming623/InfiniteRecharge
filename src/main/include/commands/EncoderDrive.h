#pragma once

//includes
#include <frc2/command/CommandBase.h>
#include <frc2/command/CommandHelper.h>
#include <frc2/command/SequentialCommandGroup.h>
#include <math.h>
#include <wpi/SmallSet.h>
//end includes 

namespace ohs2020 {

//robot information variables
const unsigned short int COUNTS_PER_ROTATION = 2048; //encoder counts per rotation, falcon500 integrated = 2048CPR
const float WHEEL_DIAMETER = 8.0f; //diameter in inches
const double GEAR_RATIO = 12.0/1.0; //gear ratio of the bot

	//calculated variables
const double CPI = (COUNTS_PER_ROTATION * GEAR_RATIO) / ((WHEEL_DIAMETER * (atan(1)*4))/*PI*/ ); //counter per rotation of this wheel
const double HORIZONTAL_CALIBRATION = 1/1; //intended/actual, because mechanim wheels skid a variable to change how many ticks are needed for horizontal traverse
	//end calculated variables

const int COUNT_THRESHOLD = 200;//accuracy threshold of counts
//end robot information variables

class EncoderDrive : public frc2::CommandHelper<frc2::CommandBase, EncoderDrive> /*frc2::CommandHelper<frc2::CommandBase, EncoderDrive> */{
	
public:
	//constructors
	EncoderDrive(int x, int y, int a);//using ticks
	EncoderDrive(double x, double y, int a);//using inches
	EncoderDrive(int x, int y);//ticks w/o rot
	EncoderDrive(double x, double y);//inches w/o rot
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

	static frc2::Command* RotateTo(double);

	//getters
	int GetX() { return m_X; }
	int GetY() { return m_Y; }
	double GetA() { return m_A; }
	//end getters

	//setters
	void SetX(int x) { m_X = x;}
	//end setters
private:

	//movement variables
	int m_X, m_Y, m_A;//x and y traverse (horizontal(x) & vertical(y)) in encoder ticks, EncoderTurnTicks(A)
	int m_InitialTicks [4];
	//end movement variables

};//end class

}//end namespace

//(!) WARN (!) change var names and method names to UPPER camel case via F2