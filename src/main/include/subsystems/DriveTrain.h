#pragma once

#include <frc2/command/SubsystemBase.h>
#include <ctre/Phoenix.h>
#include <memory>

namespace ohs2020 {
 
class DriveTrain : public frc2::SubsystemBase {

public:
	DriveTrain();

	void CartesianDrive(double x, double y, double rotation, double angle);

private:
	WPI_TalonSRX m_LeftFront, m_RightFront;
	WPI_TalonSRX m_LeftBack, m_RightBack;

};

}//namespace
