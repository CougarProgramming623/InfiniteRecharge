#include "commands/TurnToPosPID.h"

namespace ohs2020 {


std::function<double()> measurement = []()->double {    
    DebugOutF("Initial angle: " + std::to_string(Robot::Get().GetNavX()->GetYaw()));
       return (double)(Robot::Get().GetNavX()->GetYaw());
};
std::function<void(double)> output = [](double measure) { 
    DebugOutF("Initial angle: " + std::to_string(Robot::Get().GetNavX()->GetYaw()));
    Robot::Get().GetDriveTrain().CartesianDrive(0, 0, measure/2, Robot::Get().GetNavX()->GetYaw());
};
/*
TurnToPosPID::TurnToPosPID() : frc2::PIDCommand(CreateTurnController(), measurement, GetVisionAngle(), output, wpi::ArrayRef<frc2::Subsystem*>(&Robot::Get().GetDriveTrain())) {
    
    m_Angle = GetVisionAngle();

}
*/
TurnToPosPID::TurnToPosPID(double angle) : frc2::PIDCommand(CreateTurnController(), measurement, angle, output, {&Robot::Get().GetDriveTrain()}) {
    m_Angle = angle;
}

void TurnToPosPID::Initialize() {
	frc2::PIDCommand::Initialize();
	m_TurnController->SetSetpoint(m_Angle);

}

void TurnToPosPID::Execute() {
	frc2::PIDCommand::Execute();
	//Robot::Get().GetDriveTrain().CartesianDrive(0, 0, 0.4, Robot::Get().GetNavX()->GetYaw());
}

bool TurnToPosPID::IsFinished() {
	DebugOutF(std::to_string(m_TurnController->GetSetpoint()));
	DebugOutF(std::to_string((double)Robot::Get().GetNavX()->GetYaw()));
	return frc2::PIDCommand::IsFinished();//m_TurnController->GetSetpoint() - (double)Robot::Get().GetNavX()->GetYaw() == 0;
}

void TurnToPosPID::End(bool interrupted){
	DebugOutF("Turn Ended");
}

frc2::PIDController TurnToPosPID::CreateTurnController() {
    /*
	Mark PID Values:
	90 : 0.0175, 0, 0
	180 : 0.0835, 0.0, 0.01
	roughly accurate for greater than 10 : 0.085, 0.0, 0.011

	Saber PID Values:

	*/

    m_TurnController = new frc2::PIDController( 0.015, 0.0, 0.0, units::second_t(20_ms) ); 

	m_TurnController->SetTolerance( 2.0, std::numeric_limits< double >::infinity() );
	m_TurnController->EnableContinuousInput(-180.0,180.0);

    return *m_TurnController;
}

double TurnToPosPID::GetVisionAngle() {
	DebugOutF(std::to_string((double)Cob::GetValue<double>(CobKey::VISION_ANGLE)));
    return Cob::GetValue<double>(CobKey::VISION_ANGLE);
}

} // namespace