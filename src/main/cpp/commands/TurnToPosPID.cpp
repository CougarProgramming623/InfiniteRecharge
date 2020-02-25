#include "commands/TurnToPosPID.h"

#include <math.h> //for math.abs

namespace ohs2020 {


std::function<double()> measurement = []()->double {    
    return (double)(Robot::Get().GetNavX()->GetYaw());
};
std::function<void(double)> output = [](double measure) { 
	Robot::Get().GetDriveTrain().GetLBack()->Set(ControlMode::Velocity, measure * 1000);
	Robot::Get().GetDriveTrain().GetLFront()->Set(ControlMode::Velocity, measure * 1000);
	Robot::Get().GetDriveTrain().GetRFront()->Set(ControlMode::Velocity, -measure * 1000);
	Robot::Get().GetDriveTrain().GetRBack()->Set(ControlMode::Velocity, -measure * 1000);
	
};

TurnToPosPID::TurnToPosPID() : frc2::PIDCommand(CreateTurnController(), measurement, [this]{ return m_Angle + m_Offset; }, output, wpi::ArrayRef<frc2::Subsystem*>(&Robot::Get().GetDriveTrain())) {
    m_IsVision = true;
}

TurnToPosPID::TurnToPosPID(double angle) : frc2::PIDCommand(CreateTurnController(), measurement, [this]{ return m_Angle + m_Offset; }, output, {&Robot::Get().GetDriveTrain()}) {
    m_IsVision = false;
	m_Angle = angle;
}

void TurnToPosPID::Initialize() {

	Robot::Get().GetDriveTrain().GetLBack()->ConfigPeakOutputForward(0.3, 0);
	Robot::Get().GetDriveTrain().GetLBack()->ConfigPeakOutputReverse(-0.3, 0);

	Robot::Get().GetDriveTrain().GetLFront()->ConfigPeakOutputForward(0.3, 0);
	Robot::Get().GetDriveTrain().GetLFront()->ConfigPeakOutputReverse(-0.3, 0);

	Robot::Get().GetDriveTrain().GetRBack()->ConfigPeakOutputForward(0.3, 0);
	Robot::Get().GetDriveTrain().GetRBack()->ConfigPeakOutputReverse(-0.3, 0);

	Robot::Get().GetDriveTrain().GetRFront()->ConfigPeakOutputForward(0.3, 0);
	Robot::Get().GetDriveTrain().GetRFront()->ConfigPeakOutputReverse(-0.3, 0);


	if (m_IsVision){
		m_Angle = GetVisionAngle();
	}
	m_Offset = Robot::Get().GetNavX()->GetYaw();
	// m_TurnController->SetSetpoint(m_Angle);
	frc2::PIDCommand::Initialize();

}

void TurnToPosPID::Execute() {
	frc2::PIDCommand::Execute();
	//Robot::Get().GetDriveTrain().CartesianDrive(0, 0, 0.3, Robot::Get().GetNavX()->GetYaw());
}

bool TurnToPosPID::IsFinished() {
	DebugOutF("setpoint:" + std::to_string(m_Angle));
	DebugOutF("point:" + std::to_string((double)Robot::Get().GetNavX()->GetYaw()));
	DebugOutF(std::to_string(abs(m_Angle - (double)Robot::Get().GetNavX()->GetYaw())));
	return Robot::Get().GetOI().GetButtonBoard().GetRawButtonReleased(1); //GetController().AtSetpoint()
}


void TurnToPosPID::End(bool interrupted){
	DebugOutF("Turn Ended");

	frc2::PIDCommand::End(interrupted);

	Robot::Get().GetDriveTrain().GetLBack()->ConfigPeakOutputForward(1, 0);
	Robot::Get().GetDriveTrain().GetLBack()->ConfigPeakOutputReverse(-1, 0);

	Robot::Get().GetDriveTrain().GetLFront()->ConfigPeakOutputForward(1, 0);
	Robot::Get().GetDriveTrain().GetLFront()->ConfigPeakOutputReverse(-1, 0);

	Robot::Get().GetDriveTrain().GetRBack()->ConfigPeakOutputForward(1, 0);
	Robot::Get().GetDriveTrain().GetRBack()->ConfigPeakOutputReverse(-1, 0);

	Robot::Get().GetDriveTrain().GetRFront()->ConfigPeakOutputForward(1, 0);
	Robot::Get().GetDriveTrain().GetRFront()->ConfigPeakOutputReverse(-1, 0);
}

frc2::PIDController TurnToPosPID::CreateTurnController() {
    /*
	Mark PID Values:
	90 : 0.0175, 0, 0
	180 : 0.0835, 0.0, 0.01
	roughly accurate for greater than 10 : 0.085, 0.0, 0.011

	Saber PID Values:
	0.325, 0.0, 0.0125
	*/

    m_TurnController = new frc2::PIDController( 0.85, 0.0, 0.04, units::second_t(20_ms) );

	m_TurnController->SetTolerance( 2, std::numeric_limits< double >::infinity() );
	m_TurnController->EnableContinuousInput(-180.0,180.0);

    return *m_TurnController;
}

double TurnToPosPID::GetVisionAngle() {
	double x = Cob::GetValue<double>(CobKey::VISION_X);
	// double y = Cob::GetValue<double>(CobKey::VISION_X);
    return x;
}

} // namespace