#include "subsystems/DriveTrain.h"
#include "commands/DriveWithJoysticks.h"
#include "Util.h"
#include "Robot.h"
#include "ohs/RobotID.h"

#include <frc/drive/Vector2d.h>
#include <frc2/command/button/JoystickButton.h>

#include <algorithm>

const int kMAX_VELOCITY = 643;

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

	m_LeftFront.SetNeutralMode(Brake);
	m_RightFront.SetNeutralMode(Brake);
	m_LeftBack.SetNeutralMode(Brake);
	m_RightBack.SetNeutralMode(Brake);

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

	Saber PID Values:

	*/
	m_TurnController = new frc2::PIDController( 0.0125, 0.0, 0.00, units::second_t(20_ms) ); 

	m_TurnController->SetTolerance( 2.0, std::numeric_limits< double >::infinity() );
	m_TurnController->SetSetpoint(angle);
	m_TurnController->EnableContinuousInput(-180.0,180.0);

	frc2::PIDCommand* turnCmd = new frc2::PIDCommand(*m_TurnController, measurement, angle, output, wpi::ArrayRef<frc2::Subsystem*>(&Robot::Get().GetDriveTrain()));
	return turnCmd;
}
/*
frc2::MecanumControllerCommand DriveTrain::AutoDrive(frc::Trajectory trajectory) {

	std::function<frc::Pose2d()> measurement = [trajectory]()->frc::Pose2d{return trajectory.InitialPose();};

	m_XController = frc2::PIDController( 0.0, 0.0, 0.0, units::second_t(20_ms) ); 
	m_XController.SetTolerance( 1.0, std::numeric_limits< double >::infinity() );
	m_XController.EnableContinuousInput(-1.0, 1.0);

	m_YController = frc2::PIDController( 0.0, 0.0, 0.0, units::second_t(20_ms) ); 
	m_YController.SetTolerance( 1.0, std::numeric_limits< double >::infinity() );
	m_YController.EnableContinuousInput(-1.0, 1.0);
	
	

	frc::TrapezoidProfile<units::unit_t<units::meters_per_second>>::Constraints constraints{units::unit_t<units::meters_per_second> 0.0, 1.0};

	m_ThetaController = new frc::ProfiledPIDController( 0.085, 0.0, 0.011, , units::second_t(20_ms) ); 
	m_ThetaController.EnableContinuousInput(-1.0, 1.0);


}
*/
}//namespace
