#include "subsystems/DriveTrain.h"
#include "Robot.h"

#include <frc/drive/Vector2d.h>
#include <frc2/command/button/JoystickButton.h>

const int kMAX_VELOCITY = 643;

const int kFRONT_LEFT = 0;
const int kFRONT_RIGHT = 1;
const int kBACK_LEFT = 2;
const int kBACK_RIGHT = 3;


namespace ohs2020 {

DriveTrain::DriveTrain() : m_LeftFront(0), m_RightFront(0), m_LeftBack(0), m_RightBack(0) {}

void Normalize(wpi::MutableArrayRef<double> wheelSpeeds) {
	double maxMagnitude = std::abs(wheelSpeeds[0]);

	for (size_t i = 1; i < wheelSpeeds.size(); i++) {
    	double temp = std::abs(wheelSpeeds[i]);
    if (maxMagnitude < temp) {
      maxMagnitude = temp;
    }
  }
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
	double wheelSpeeds[4] ;
	
	wheelSpeeds[kFRONT_LEFT] = input.y + input.x + rotation;
	wheelSpeeds[kFRONT_RIGHT] = input.y - input.x - rotation;
	wheelSpeeds[kBACK_LEFT] = input.y - input.x + rotation;
	wheelSpeeds[kBACK_RIGHT] = input.y + input.x  - rotation;

	Normalize(wheelSpeeds);

	
	if(Robot::Get().GetOI().GetVelocityMode()) {

		m_LeftFront.Set(ControlMode::Velocity, wheelSpeeds[kFRONT_LEFT] * kMAX_VELOCITY);
		m_LeftBack.Set(ControlMode::Velocity,wheelSpeeds[kBACK_LEFT] * kMAX_VELOCITY);
		m_RightFront.Set(ControlMode::Velocity, wheelSpeeds[kFRONT_RIGHT] * kMAX_VELOCITY);
		m_RightBack.Set(ControlMode::Velocity, wheelSpeeds[kBACK_RIGHT] * kMAX_VELOCITY);

	} else {
		
		m_LeftFront.Set(ControlMode::PercentOutput, wheelSpeeds[kFRONT_LEFT]);
		m_LeftBack.Set(ControlMode::PercentOutput,wheelSpeeds[kBACK_LEFT]);
		m_RightFront.Set(ControlMode::PercentOutput, wheelSpeeds[kFRONT_RIGHT]);
		m_RightBack.Set(ControlMode::PercentOutput, wheelSpeeds[kBACK_RIGHT]);

	}

} //CartesianDrive()
}//namespace
