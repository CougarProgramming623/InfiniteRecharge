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

TurnToPosPID::TurnToPosPID() : frc2::PIDCommand(CreateTurnController(), measurement, [this]{ return m_Angle; }, output, wpi::ArrayRef<frc2::Subsystem*>(&Robot::Get().GetDriveTrain())) {
    
    m_Angle = 3000;//GetVisionAngle();

}

TurnToPosPID::TurnToPosPID(double angle) : frc2::PIDCommand(CreateTurnController(), measurement, angle, output, {&Robot::Get().GetDriveTrain()}) {
    m_Angle = angle;
}

void TurnToPosPID::Initialize() {

	Robot::Get().GetDriveTrain().GetLBack()->ConfigPeakOutputForward(0.4, 0);
	Robot::Get().GetDriveTrain().GetLBack()->ConfigPeakOutputReverse(0.4, 0);

	Robot::Get().GetDriveTrain().GetLFront()->ConfigPeakOutputForward(0.4, 0);
	Robot::Get().GetDriveTrain().GetLFront()->ConfigPeakOutputForward(0.4, 0);

	Robot::Get().GetDriveTrain().GetRBack()->ConfigPeakOutputForward(0.4, 0);
	Robot::Get().GetDriveTrain().GetRBack()->ConfigPeakOutputForward(0.4, 0);

	Robot::Get().GetDriveTrain().GetRFront()->ConfigPeakOutputForward(0.4, 0);
	Robot::Get().GetDriveTrain().GetRFront()->ConfigPeakOutputForward(0.4, 0);


	if (m_Angle == 3000){
		m_Angle = GetVisionAngle();
	}
	// m_TurnController->SetSetpoint(m_Angle);
	frc2::PIDCommand::Initialize();

}

void TurnToPosPID::Execute() {
	frc2::PIDCommand::Execute();
	//Robot::Get().GetDriveTrain().CartesianDrive(0, 0, 0.4, Robot::Get().GetNavX()->GetYaw());
}

bool TurnToPosPID::IsFinished() {
	DebugOutF("setpoint:" + std::to_string(m_Angle));
	DebugOutF("point:" + std::to_string((double)Robot::Get().GetNavX()->GetYaw()));
	DebugOutF(std::to_string(abs(m_Angle - (double)Robot::Get().GetNavX()->GetYaw())));
	return abs(m_Angle - (double)Robot::Get().GetNavX()->GetYaw()) < 2;
}

void TurnToPosPID::End(bool interrupted){
	DebugOutF("Turn Ended");
	frc2::PIDCommand::End(interrupted);
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

    m_TurnController = new frc2::PIDController( 0.325, 0.0, 0.0125, units::second_t(20_ms) );

	m_TurnController->SetTolerance( 2.0, std::numeric_limits< double >::infinity() );
	m_TurnController->EnableContinuousInput(-180.0,180.0);

    return *m_TurnController;
}

double TurnToPosPID::GetVisionAngle() {
	// DebugOutF(std::to_string((double)Cob::GetValue<double>(CobKey::VISION_ANGLE)));
	double x = Cob::GetValue<double>(CobKey::VISION_X);
	// double y = Cob::GetValue<double>(CobKey::VISION_X);
    return x;
}

} // namespace