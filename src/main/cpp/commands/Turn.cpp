#include "commands/Turn.h"
#include "Util.h"
#include "Robot.h"

#include <wpi/ArrayRef.h>
//#include <frc/Command>

namespace ohs623 {

Turn::Turn(double angle) : frc2::PIDCommand(*m_TurnController, measurement, m_Angle, output, wpi::ArrayRef<frc2::Subsystem*>(&ohs2020::Robot::Get().GetDriveTrain())), frc::PIDOutput() {
	//AddRequirements(wpi::ArrayRef<frc2::Subsystem*>(&ohs2020::Robot::Get().GetDriveTrain()));

	m_RotToAngleRate = 0.5;
	m_Angle = angle;

	//PID Vars
	m_KP = 0.05;
	m_KI = 0.00;
	m_KD = 0.00;
	//(!) adjust later


}//constructor

void Turn::Initialize() {
	m_TurnController = new frc2::PIDController( m_KP, m_KI, m_KD, units::second_t(20_ms) ); //ohs2020::Robot::Get().GetDriveTrain(), this,

	//set of sets
	m_TurnController->SetIntegratorRange(0.0f , 360.0f);
	m_TurnController->EnableContinuousInput(-1.0,1.0); 
	m_TurnController->SetTolerance( 2.0, std::numeric_limits< double >::infinity() );
	//

}//initialization method

void Turn::Execute() {
	DebugOutF( std::to_string((double)ohs2020::Robot::Get().GetNavX()->GetYaw() ));
	ohs2020::Robot::Get().GetDriveTrain().CartesianDrive(0, 0, m_RotToAngleRate / 2, ohs2020::Robot::Get().GetNavX()->GetYaw());
	
}//execute method

bool Turn::IsFinished() {
	return m_TurnController->AtSetpoint();


}//get method, returns status of timer

void Turn::End(bool interrupted) {
	DebugOutF("Turn Finished");
}//tell finish end

void Turn::PIDWrite(double output) {
	m_RotToAngleRate = output;
}

}//end namespace