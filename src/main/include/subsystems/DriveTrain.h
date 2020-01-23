#pragma once

#include <frc2/command/SubsystemBase.h>
#include <ctre/Phoenix.h>
#include <memory>

namespace ohs2020 {
 
class DriveTrain : frc2::SubsystemBase {

public:
	DriveTrain();

	void CartesianDrive(double x, double y, double rotation, double angle);


	virtual std::unique_ptr<frc2::Command> TransferOwnership() &&;

private:
	WPI_TalonSRX m_LeftFront, m_RightFront;
	WPI_TalonSRX m_LeftBack, m_RightBack;

};

}//namespace
