#include "subsystems/DriveTrain.h"
#include "commands/DriveWithJoysticks.h"
#include "Util.h"
#include "Robot.h"
#include "ohs/RobotID.h"

#include <frc/drive/Vector2d.h>
#include <frc2/command/button/JoystickButton.h>

#include <algorithm>


const int kFRONT_LEFT = 0;
const int kFRONT_RIGHT = 1;
const int kBACK_LEFT = 2;
const int kBACK_RIGHT = 3;


namespace ohs2020 {

using namespace ohs623;

DriveTrain::DriveTrain() : m_LeftFront(RobotID::GetID(FRONT_LEFT)), m_RightFront(RobotID::GetID(FRONT_RIGHT)), m_LeftBack(RobotID::GetID(BACK_LEFT)), m_RightBack(RobotID::GetID(BACK_RIGHT)) {

	m_RightFront.SetInverted(true);
	m_RightBack.SetInverted(true);

}
  
void DriveTrain::Init(){
	SetDefaultCommand(Drive()); 

	SetBrakeMode(true);

	UseVelocityPID();

	enableJoySticks = true;
}

void DriveTrain::SetBrakeMode(bool on){
	if(on){
		Robot::Get().GetDriveTrain().GetLFront()->SetNeutralMode(Brake);
		Robot::Get().GetDriveTrain().GetRFront()->SetNeutralMode(Brake);
		Robot::Get().GetDriveTrain().GetLBack()->SetNeutralMode(Brake);
		Robot::Get().GetDriveTrain().GetRBack()->SetNeutralMode(Brake);
	}else{
		Robot::Get().GetDriveTrain().GetLFront()->SetNeutralMode(Coast);
		Robot::Get().GetDriveTrain().GetRFront()->SetNeutralMode(Coast);
		Robot::Get().GetDriveTrain().GetLBack()->SetNeutralMode(Coast);
		Robot::Get().GetDriveTrain().GetRBack()->SetNeutralMode(Coast);
	}
}

void DriveTrain::UsePositionPID(){
	DebugOutF("ERROR: USING POSITION PID");
	DriveTrain::SetPID(25, 0.025, 0.0, 0.0, 0.0);
}

void DriveTrain::UseVelocityPID(){
	DebugOutF("ERROR:USING VELOCITY PID");
	DriveTrain::SetPID(50, 0.0, 0.0, 0.0, 0.05);
}

void DriveTrain::SetPID(double E, double P, double I, double D, double F){

	DebugOutF("ERROR:SET EPIDF TO: "+ std::to_string(E) + "/"+ std::to_string(P) + "/"+ std::to_string(I) + "/"+ std::to_string(D) + "/"+ std::to_string(F)  );

	GetLFront()->ConfigAllowableClosedloopError(0,E,0);
	GetRFront()->ConfigAllowableClosedloopError(0,E,0);
	GetLBack()->ConfigAllowableClosedloopError(0,E,0);
	GetRBack()->ConfigAllowableClosedloopError(0,E,0);

	GetLFront()->Config_kP(0,P,0);
	GetRFront()->Config_kP(0,P,0);
	GetLBack()->Config_kP(0,P,0);
	GetRBack()->Config_kP(0,P,0);

	GetLFront()->Config_kI(0,I,0);
	GetRFront()->Config_kI(0,I,0);
	GetLBack()->Config_kI(0,I,0);
	GetRBack()->Config_kI(0,I,0);

	GetLFront()->Config_kD(0,D,0);
	GetRFront()->Config_kD(0,D,0);
	GetLBack()->Config_kD(0,D,0);
	GetRBack()->Config_kD(0,D,0);

	GetLFront()->Config_kF(0,F,0);
	GetRFront()->Config_kF(0,F,0);
	GetLBack()->Config_kF(0,F,0);
	GetRBack()->Config_kF(0,F,0);

}

/*void DriveTrain::InitDefaultCommand(){

}*/

void Normalize(wpi::MutableArrayRef<double> wheelSpeeds) {
	double maxMagnitude = *std::max_element(wheelSpeeds.begin(), wheelSpeeds.end());

	if (maxMagnitude > 1) {
		for (size_t i = 0; i < wheelSpeeds.size(); i++) {
			wheelSpeeds[i] = wheelSpeeds[i] / maxMagnitude;
		}
	}
} //Normalize()

void DriveTrain::CartesianDrive(double y, double x, double rotation, double angle) {
	//source: WPILib
	//same code found in CartesianDrive in the WPI Library but adapted for being used in Velocity Mode
	frc::Vector2d input{x, y};
	input.Rotate(angle);
	double wheelSpeeds[4];
	
	wheelSpeeds[kFRONT_LEFT] = input.y + input.x + rotation;
	wheelSpeeds[kFRONT_RIGHT] = input.y - input.x - rotation;
	wheelSpeeds[kBACK_LEFT] = input.y - input.x + rotation;
	wheelSpeeds[kBACK_RIGHT] = input.y + input.x - rotation;

	Normalize(wheelSpeeds);

	if (Robot::Get().GetOI().GetVelocityMode()) {

		m_LeftFront.Set(ControlMode::Velocity, wheelSpeeds[kFRONT_LEFT] * kMAX_VELOCITY);
		m_LeftBack.Set(ControlMode::Velocity, wheelSpeeds[kBACK_LEFT] * kMAX_VELOCITY);
		m_RightFront.Set(ControlMode::Velocity, wheelSpeeds[kFRONT_RIGHT] * kMAX_VELOCITY);
		m_RightBack.Set(ControlMode::Velocity, wheelSpeeds[kBACK_RIGHT] * kMAX_VELOCITY);

	} else {
		
		m_LeftFront.Set(ControlMode::PercentOutput, wheelSpeeds[kFRONT_LEFT]);
		m_LeftBack.Set(ControlMode::PercentOutput, wheelSpeeds[kBACK_LEFT]);
		m_RightFront.Set(ControlMode::PercentOutput, wheelSpeeds[kFRONT_RIGHT]);
		m_RightBack.Set(ControlMode::PercentOutput, wheelSpeeds[kBACK_RIGHT]);

		//DebugOutF(std::to_string(wheelSpeeds[kFRONT_LEFT]));
	}

} //CartesianDrive()

frc2::PIDCommand* DriveTrain::TurnToPos(double angle) {

	std::function<double()> measurement = []()->double{return (double)(Robot::Get().GetNavX()->GetYaw());};
	std::function<void(double)> output = [this](double measure) { 
		CartesianDrive(0, 0, measure/2, Robot::Get().GetNavX()->GetYaw());
		//DebugOutF(std::to_string(measure/2)); 
	};

	/*
	Mark PID Values:
	90 : 0.0175, 0, 0
	180 : 0.0835, 0.0, 0.01
	roughly accurate for greater than 10 : 0.085, 0.0, 0.011
	*/
	m_TurnController = new frc2::PIDController( 0.085, 0.0, 0.011, units::second_t(20_ms) ); 

	m_TurnController->SetTolerance( 1.0, std::numeric_limits< double >::infinity() );
	m_TurnController->SetSetpoint(angle);
	m_TurnController->EnableContinuousInput(-180.0,180.0);

	frc2::PIDCommand* turnCmd = new frc2::PIDCommand(*m_TurnController, measurement, angle, output, wpi::ArrayRef<frc2::Subsystem*>(&Robot::Get().GetDriveTrain()));
	return turnCmd;
}
}//namespace
