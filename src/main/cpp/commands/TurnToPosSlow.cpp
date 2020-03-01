#include "commands/TurnToPosSlow.h"
#include "Robot.h"

namespace ohs2020 {

TurnToPosSlow::TurnToPosSlow() {
	AddRequirements(&Robot::Get().GetDriveTrain());

}

void TurnToPosSlow::Initialize() {
    m_Angle = GetVisionAngle();
    m_StartNeg = m_Angle < 0;
}

void TurnToPosSlow::Execute() {
    m_Angle = GetVisionAngle();
    
    int velocity = 2000;

    if(abs(m_Angle) < 10 ) 
        velocity = 900;

    if (m_StartNeg) {
        Robot::Get().GetDriveTrain().GetLBack()->Set(ControlMode::Velocity, -velocity);
        Robot::Get().GetDriveTrain().GetLFront()->Set(ControlMode::Velocity, -velocity);
        Robot::Get().GetDriveTrain().GetRFront()->Set(ControlMode::Velocity, velocity);
        Robot::Get().GetDriveTrain().GetRBack()->Set(ControlMode::Velocity, velocity);
    } else {
        Robot::Get().GetDriveTrain().GetLBack()->Set(ControlMode::Velocity, velocity);
        Robot::Get().GetDriveTrain().GetLFront()->Set(ControlMode::Velocity, velocity);
        Robot::Get().GetDriveTrain().GetRFront()->Set(ControlMode::Velocity, -velocity);
        Robot::Get().GetDriveTrain().GetRBack()->Set(ControlMode::Velocity, -velocity);
    }
}

bool TurnToPosSlow::IsFinished() {
    m_Angle = GetVisionAngle();
    double tolerance = 1;
    if(!frc::DriverStation::GetInstance().IsAutonomous() && !Robot::Get().GetOI().GetButtonBoard().GetRawButton(1)) 
        return true;
    if (m_StartNeg)
        return m_Angle >= -tolerance;
    else
        return m_Angle <= tolerance;
}

void TurnToPosSlow::End(bool interrupted) {

	DebugOutF("Turn Ended");
    Robot::Get().GetDriveTrain().GetLBack()->Set(ControlMode::Velocity, 0);
    Robot::Get().GetDriveTrain().GetLFront()->Set(ControlMode::Velocity, 0);
    Robot::Get().GetDriveTrain().GetRFront()->Set(ControlMode::Velocity, 0);
    Robot::Get().GetDriveTrain().GetRBack()->Set(ControlMode::Velocity, 0);

}

double TurnToPosSlow::GetVisionAngle() {
	double x = Cob::GetValue<double>(CobKey::VISION_X);
    return x;
}

}