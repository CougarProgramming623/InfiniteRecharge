#pragma once

#include <frc2/command/SubsystemBase.h>
#include <frc2/command/PIDCommand.h>
#include <ctre/Phoenix.h>
#include <memory>


namespace ohs2020 {	
 
class DriveTrain : public frc2::SubsystemBase {

public:

	DriveTrain();

	const static int kMAX_VELOCITY = 6380/60/10*2048;//RPM->Convert to RPS->Convert to RP100MS->Convert to TP100MS

	void Init();
	//frc2::Command* GetDefaultCommand() const override;

	void CartesianDrive(double x, double y, double rotation, double angle);
	
	void UseVelocityPID();

	void UsePositionPID();

	void SetPID(double E, double P, double I, double D, double F);

	void SetBrakeMode(bool on);

	//virtual void InitDefaultCommand() override;  

	frc2::PIDCommand* TurnToPos(double angle);
	
	//getters for direct access to motors
	WPI_TalonSRX* GetLFront() {return &m_LeftFront;}
	WPI_TalonSRX* GetRFront() {return &m_RightFront;}
	WPI_TalonSRX* GetLBack() {return &m_LeftBack;}
	WPI_TalonSRX* GetRBack() {return &m_RightBack;}
	
protected:
	
	//virtual std::unique_ptr<frc2::Command> TransferOwnership() && override;

private:
	WPI_TalonSRX m_LeftFront, m_RightFront;
	WPI_TalonSRX m_LeftBack, m_RightBack;

	frc2::PIDController* m_TurnController;

};

}//namespace
