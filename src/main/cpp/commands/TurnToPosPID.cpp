#include "commands/TurnToPosPID.h"

namespace ohs2020 {

std::function<double()> measurement = []()->double{return (double)(Robot::Get().GetNavX()->GetYaw());};
std::function<void(double)> output = [](double measure) { 
	Robot::Get().GetDriveTrain().CartesianDrive(0, 0, measure/2, Robot::Get().GetNavX()->GetYaw());
};

TurnToPosPID::TurnToPosPID() : frc2::PIDCommand(*m_TurnController, measurement, angle, output, wpi::ArrayRef<frc2::Subsystem*>(&Robot::Get().GetDriveTrain())) {
    

	//frc2::PIDCommand* turnCmd = new frc2::PIDCommand(*m_TurnController, measurement, angle, output, wpi::ArrayRef<frc2::Subsystem*>(&Robot::Get().GetDriveTrain()));
}

TurnToPosPID::TurnToPosPID(double angle) : frc2::PIDCommand(*m_TurnController, measurement, angle, output, wpi::ArrayRef<frc2::Subsystem*>(&Robot::Get().GetDriveTrain())){

}

frc2::PIDController TurnToPosPID::CreateTurnController() {
    /*
	Mark PID Values:
	90 : 0.0175, 0, 0
	180 : 0.0835, 0.0, 0.01
	roughly accurate for greater than 10 : 0.085, 0.0, 0.011

	Saber PID Values:

	*/

    m_TurnController = new frc2::PIDController( 0.085, 0.0, 0.011, units::second_t(20_ms) ); 

	m_TurnController->SetTolerance( 2.0, std::numeric_limits< double >::infinity() );
	m_TurnController->SetSetpoint(angle);
	m_TurnController->EnableContinuousInput(-180.0,180.0);
    
    return *m_TurnController;
}

} // namespace