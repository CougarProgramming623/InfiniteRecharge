#include "subsystems/DriveTrain.h"

namespace ohs2020 {

DriveTrain::DriveTrain() : m_LeftFront(0), m_RightFront(0), m_LeftBack(0), m_RightBack(0) {

}

void DriveTrain::CartesianDrive(double x, double y, double rotation, double angle) {
    m_LeftFront.Set(ControlMode::Velocity, 0);
    m_RightFront.Set(ControlMode::Velocity, 0);
    m_LeftBack.Set(ControlMode::Velocity, 0);
    m_RightBack.Set(ControlMode::Velocity, 0);
}

}