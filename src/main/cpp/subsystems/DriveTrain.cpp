#include "subsystems/DriveTrain.h"

namespace ohs2020{

DriveTrain :: DriveTrain() : m_LeftFront(0), m_RightFront(0), m_LeftRear(0), m_RightRear(0){
    
}//default constructor

void DriveTrain :: CartesianDrive(double x, double y, double rotation, double angle){
    m_LeftFront.Set(ControlMode::Velocity,0);
    m_RightFront.Set(ControlMode::Velocity,0);
    m_LeftRear.Set(ControlMode::Velocity,0);
    m_RightRear.Set(ControlMode::Velocity,0);
}

}