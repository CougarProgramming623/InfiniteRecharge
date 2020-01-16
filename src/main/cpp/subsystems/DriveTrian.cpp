#include "subsystems/DriveTrain.h"

namespace ohs2020 {

DriveTrain::DriveTrain() : m_LeftFront(0), m_RightFront(0), m_LeftBack(0), m_RightBack(0) {


}

void DriveTrain::CartesianDrive(double x, double y, double rotation, double angle) { 
	m_FrontLeft.Set(ControlMode::Velocity, 0);
	m_FrontRight.Set(ControlMode::Velocity, 0);
	m_BackLeft.Set(ControlMode::Velocity, 0);
	m_BackRight.Set(ControlMode::Velocity, 0);
}

}//namespace
