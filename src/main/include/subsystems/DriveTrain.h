#pragma once

#include <frc2/command/SubsystemBase.h>
#include <frc2/command/PIDCommand.h>
#include <ctre/Phoenix.h>
#include <memory>


namespace ohs2020 {	
 
class DriveTrain : public frc2::SubsystemBase {

public:
	DriveTrain();


	void Init();
	//frc2::Command* GetDefaultCommand() const override;

	void CartesianDrive(double x, double y, double rotation, double angle);

	//virtual void InitDefaultCommand() override;  

	frc2::PIDCommand* TurnToPos(double angle);

	void SetBrakeMode();
	void UseVelocityPID();
	void UsePositionPID();
	void UseMagicPID(double max);
	void SetPID(double, double, double, double, double);
	void SetBrakeMode(bool);
	
	//getters for direct access to motors
	BaseTalon* GetLFront() { return m_FrontLeft.get(); }
	BaseTalon* GetRFront() { return m_FrontRight.get(); }
	BaseTalon* GetLBack() { return m_BackLeft.get(); }
	BaseTalon* GetRBack() { return m_BackRight.get(); }

protected:
	
	//virtual std::unique_ptr<frc2::Command> TransferOwnership() && override;

private:
	std::unique_ptr<BaseTalon> m_FrontLeft, m_FrontRight;
	std::unique_ptr<BaseTalon> m_BackLeft, m_BackRight;

	frc2::PIDController* m_TurnController;
	/*
	frc2::PIDController m_XController;
	frc2::PIDController m_YController;
	frc::ProfiledPIDController<units::radians> m_ThetaController;

	frc::MecanumDriveKinematics kinematics;
*/
};

}//namespace
