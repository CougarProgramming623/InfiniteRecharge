#pragma once

#include <frc2/command/SubsystemBase.h>
#include <frc2/command/PIDCommand.h>
//#include <frc2/command/MecanumControllerCommand.h>
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

	frc2::PIDCommand TurnToPos(double angle);
	//frc2::MecanumControllerCommand AutoDrive(frc::Trajectory);

protected:
	
	//virtual std::unique_ptr<frc2::Command> TransferOwnership() && override;

private:
	WPI_TalonSRX m_LeftFront, m_RightFront;
	WPI_TalonSRX m_LeftBack, m_RightBack;

	frc2::PIDController* m_TurnController;
	/*
	frc2::PIDController m_XController;
	frc2::PIDController m_YController;
	frc::ProfiledPIDController<units::radians> m_ThetaController;

	frc::MecanumDriveKinematics kinematics;
*/
};

}//namespace
