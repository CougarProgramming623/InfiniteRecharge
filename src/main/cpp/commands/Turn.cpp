#include "commands/Turn.h"
#include "Util.h"
#include "Robot.h"

#include <wpi/ArrayRef.h>
//#include <frc/Command>

namespace ohs623 {

Turn::Turn(double angle){
	AddRequirements( wpi::ArrayRef<frc2::Subsystem*>(&ohs2020::Robot.Get().GetDriveTrain()) );

	m_RotToAngleRate = 0.5;
	m_Angle = angle;

	//PID Vars
	m_KP = 0.00;
	m_KI = 0.00;
	m_KD = 0.00;
	//(!) adjust later

	frc2::Command.WithTimeout(2);
	//frc::Command.SetTimeout(2);

}//constructor

void Turn::Initialize() {
	m_TurnController = new PIDController( m_KP, m_KI, m_KD, ohs2020::Robot.Get().GetDriveTrain(), this, 0.05);

	//set of sets
	m_TurnController->SetIntegratorRange(-180.0f,180.0f);
	m_TurnController->EnableContinuousInput(-1.0,1.0); 
	m_TurnController->SetTolerance( 2.0, std::numeric_limits< double >::infinity() );
	m_TurnController->SetSetPoint(m_Angle);
	//

}//initialization method

void Turn::Execute() {
	DebugOutF( std::to_string( ohs2020::Robot.Get().GetNavX() ) );
	ohs2020::Robot.Get().GetDriveTrain().CartesianDrive(0, 0, m_RotToAngleRate / 2, ohs2020::Robot.Get().GetNavX()->GetYaw());
	
}//execute method

bool Turn::IsFinished() {
	//return frc::Command.IsTimedOut();

	return true;

}//get method, returns status of timer

void Turn::End(bool interrupted) {
	DebugOutF("Turn Finished");
}//tell finish end

void Turn::PIDWrite(double output) {
	m_RotToAngleRate = ouptut;
}

}//end namespace