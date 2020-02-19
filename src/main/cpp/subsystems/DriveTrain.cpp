#include "subsystems/DriveTrain.h"
#include "commands/DriveWithJoysticks.h"
#include "Util.h"
#include "Robot.h"
#include "ohs/RobotID.h"
#include "ohs/Log.h"

#include <frc/smartdashboard/SmartDashboard.h>
#include <frc/drive/Vector2d.h>
#include <frc2/command/button/JoystickButton.h>

#include <algorithm>

const int kMAX_VELOCITY = 6380/60/10*2048;//RPM->Convert to RPS->Convert to RP100MS->Convert to TP100MS

const int kFRONT_LEFT = 0;
const int kFRONT_RIGHT = 1;
const int kBACK_LEFT = 2;
const int kBACK_RIGHT = 3;


namespace ohs2020 {

using namespace ohs623;

DriveTrain::DriveTrain() {
	OHS_DEBUG([](auto& f) { f << "DriveTrain::DriveTrain()"; });
	m_FrontLeft.reset(RobotID::InitMotor(FRONT_LEFT));
	m_FrontRight.reset(RobotID::InitMotor(FRONT_RIGHT));
	m_BackLeft.reset(RobotID::InitMotor(BACK_LEFT));
	m_BackRight.reset(RobotID::InitMotor(BACK_RIGHT));

	m_FrontRight->SetInverted(true);
	m_BackRight->SetInverted(true);

}

void DriveTrain::Init() {
	SetDefaultCommand(Drive());

	m_FrontLeft->SetNeutralMode(ctre::phoenix::motorcontrol::NeutralMode::Brake);
	m_FrontRight->SetNeutralMode(ctre::phoenix::motorcontrol::NeutralMode::Brake);
	m_BackLeft->SetNeutralMode(ctre::phoenix::motorcontrol::NeutralMode::Brake);
	m_BackRight->SetNeutralMode(ctre::phoenix::motorcontrol::NeutralMode::Brake);

	//PID Configuration

	m_FrontLeft->ConfigAllowableClosedloopError(0,50,0);
	m_FrontRight->ConfigAllowableClosedloopError(0,50,0);
	m_BackLeft->ConfigAllowableClosedloopError(0,50,0);
	m_BackRight->ConfigAllowableClosedloopError(0,50,0);

	double P = 0.0;
	double I = 0.0;
	double D = 0.0;
	double F = 0.05;

	m_FrontLeft->Config_kP(0,P,0);
	m_FrontRight->Config_kP(0,P,0);
	m_BackLeft->Config_kP(0,P,0);
	m_BackRight->Config_kP(0,P,0);

	m_FrontLeft->Config_kI(0,I,0);
	m_FrontRight->Config_kI(0,I,0);
	m_BackLeft->Config_kI(0,I,0);
	m_BackRight->Config_kI(0,I,0);

	m_FrontLeft->Config_kD(0,D,0);
	m_FrontRight->Config_kD(0,D,0);
	m_BackLeft->Config_kD(0,D,0);
	m_BackRight->Config_kD(0,D,0);

	m_FrontLeft->Config_kF(0,F,0);
	m_FrontRight->Config_kF(0,F,0);
	m_BackLeft->Config_kF(0,F,0);
	m_BackRight->Config_kF(0,F,0);

}

void Normalize(wpi::MutableArrayRef<double> wheelSpeeds) {
	double maxMagnitude = *std::max_element(wheelSpeeds.begin(), wheelSpeeds.end());

	if (maxMagnitude > 1) {
		for (size_t i = 0; i < wheelSpeeds.size(); i++) {
			wheelSpeeds[i] = wheelSpeeds[i] / maxMagnitude;
		}
	}
} //Normalize()

void DriveTrain::CartesianDrive(double y, double x, double rotation, double angle) {

	//DebugOutF("Mode: "+ std::to_string(m_FrontLeft->GetSelectedSensorVelocity()));
	DebugOutF("VDiff: "+ std::to_string( m_FrontLeft->GetClosedLoopTarget() - m_FrontLeft->GetSelectedSensorVelocity()/4 ));

	SmartDashboard::PutNumber("PIDVELOCITY", m_FrontLeft->GetSelectedSensorVelocity());

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
		DebugOutF("Using V");
		m_FrontLeft->Set(ControlMode::Velocity, wheelSpeeds[kFRONT_LEFT] * kMAX_VELOCITY);
		m_BackLeft->Set(ControlMode::Velocity, wheelSpeeds[kBACK_LEFT] * kMAX_VELOCITY);
		m_FrontRight->Set(ControlMode::Velocity, wheelSpeeds[kFRONT_RIGHT] * kMAX_VELOCITY);
		m_BackRight->Set(ControlMode::Velocity, wheelSpeeds[kBACK_RIGHT] * kMAX_VELOCITY);

	} else {
		
		m_FrontLeft->Set(ControlMode::PercentOutput, wheelSpeeds[kFRONT_LEFT]);
		m_BackLeft->Set(ControlMode::PercentOutput, wheelSpeeds[kBACK_LEFT]);
		m_FrontRight->Set(ControlMode::PercentOutput, wheelSpeeds[kFRONT_RIGHT]);
		m_BackRight->Set(ControlMode::PercentOutput, wheelSpeeds[kBACK_RIGHT]);

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
