//includes
#include "commands/EncoderDrive.h"
#include "Robot.h"
#include "subsystems/DriveTrain.h"
#include "Util.h"

#include <math.h> //for math.abs
#include <memory>
#include <frc/drive/Vector2d.h>  //for mechanum calculations
#include <wpi/ArrayRef.h>
//end includes

namespace ohs2020 {

//EncoderDrive

//constructors
EncoderDrive::EncoderDrive(int x, int y, int a){
	m_X = x*HORIZONTAL_CALIBRATION;
	m_Y = y;
	m_A = a;
	//m_A = 0;
	//if (a != 0)
	//	m_A = ( a-4 ) * 2000/7;
	 
	// angle calculation: ( degrees-4 ) * 2000/7
	AddRequirements(wpi::ArrayRef<frc2::Subsystem*>(&Robot::Get().GetDriveTrain()));

} //base constructor

EncoderDrive::EncoderDrive(double x, double y, int a) : EncoderDrive(static_cast <int> (x*CPI), static_cast <int> (y*CPI), 0) {} 
//constructor for inches w/ angle

EncoderDrive::EncoderDrive(int x, int y) : EncoderDrive(x, y, 0) {}
//constructor for tick movement w/o angle

EncoderDrive::EncoderDrive(double x, double y) : EncoderDrive(x, y, 0) {}
//constructor for inch movement w/o angle

//end constructors

//override commands
void EncoderDrive::Initialize() {

	m_InitialTicks[0] = Robot::Get().GetDriveTrain().GetLFront()->GetSelectedSensorPosition();
	m_InitialTicks[1] = Robot::Get().GetDriveTrain().GetRFront()->GetSelectedSensorPosition();
	m_InitialTicks[2] = Robot::Get().GetDriveTrain().GetLBack()->GetSelectedSensorPosition();
	m_InitialTicks[3] = Robot::Get().GetDriveTrain().GetRBack()->GetSelectedSensorPosition();

	Robot::Get().GetDriveTrain().GetLFront()->SetNeutralMode(Coast);
	Robot::Get().GetDriveTrain().GetRFront()->SetNeutralMode(Coast);
	Robot::Get().GetDriveTrain().GetLBack()->SetNeutralMode(Coast);
	Robot::Get().GetDriveTrain().GetRBack()->SetNeutralMode(Coast);

	Robot::Get().GetDriveTrain().UsePositionPID();
}//starts motor turn

bool EncoderDrive::IsFinished() {

	return
		abs( (m_InitialTicks[0] + m_Y + m_X + m_A) - Robot::Get().GetDriveTrain().GetLFront()->GetSelectedSensorPosition() ) < COUNT_THRESHOLD &&
		abs( (m_InitialTicks[1] + m_Y - m_X - m_A) - Robot::Get().GetDriveTrain().GetRFront()->GetSelectedSensorPosition() ) < COUNT_THRESHOLD &&
		abs( (m_InitialTicks[2] + m_Y - m_X + m_A) - Robot::Get().GetDriveTrain().GetLBack()->GetSelectedSensorPosition() ) < COUNT_THRESHOLD &&
		abs( (m_InitialTicks[3] + m_Y + m_X - m_A) - Robot::Get().GetDriveTrain().GetRBack()->GetSelectedSensorPosition() ) < COUNT_THRESHOLD;


	//(!)NOTE: MORE GRAVEYARD CODE

	//DebugOutF("DIFF:"+ std::to_string(m_Y + m_A + m_InitialTicks[0] + m_X) + "|" + std::to_string(Robot::Get().GetDriveTrain().GetLFront()->GetSelectedSensorPosition()) + "|" + std::to_string(abs((m_InitialTicks + m_X + m_Y + m_A) - Robot::Get().GetDriveTrain().GetLFront()->GetSelectedSensorPosition()) <= COUNT_THRESHOLD));
	//DebugOutF("MOTOR%: "+ std::to_string(Robot::Get().GetDriveTrain().GetLFront()->GetMotorOutputPercent()));

	//if( m_X + m_Y + m_A < 0 ){
	//	return Robot::Get().GetDriveTrain().GetRFront()->GetSelectedSensorPosition() >= m_A + m_InitialTicks[0];
				//&& abs(Robot::Get().GetDriveTrain().GetRFront()->GetSelectedSensorPosition()) >= abs(m_Y - m_A + m_InitialTicks - m_X)
				//&& abs(Robot::Get().GetDriveTrain().GetLBack()->GetSelectedSensorPosition()) >= abs(m_Y + m_A + m_InitialTicks - m_X)
				//&& abs(Robot::Get().GetDriveTrain().GetLBack()->GetSelectedSensorPosition()) >= abs(m_Y - m_A + m_InitialTicks + m_X);
	//} else {
		//return Robot::Get().GetDriveTrain().GetLFront()->GetSelectedSensorPosition() >= (m_Y + m_A + m_InitialTicks + m_X);
	//}

}//returns true when encoderTicks is equals to or greater than target

void EncoderDrive::Execute() {
	//DebugOutF("DIFF:"+ std::to_string(m_Y + m_A + m_InitialTicks[0] + m_X) + "|" + std::to_string(Robot::Get().GetDriveTrain().GetLFront()->GetSelectedSensorPosition()) + "|" + std::to_string(abs((m_InitialTicks[0] + m_X + m_Y + m_A) - Robot::Get().GetDriveTrain().GetLFront()->GetSelectedSensorPosition()) <= COUNT_THRESHOLD));
	//DebugOutF("MOTOR%: "+ std::to_string(Robot::Get().GetDriveTrain().GetLFront()->GetMotorOutputPercent()));

	int max = m_X + m_A + m_Y;
	if (max < abs(m_Y - m_X - m_A))
		max = m_Y - m_X - m_A;
	
	double maxSpeed = 0.3;

	//(!)NOTE: IGNORE THIS GRAVEYARD OF CODE

	//Robot::Get().GetDriveTrain().GetLFront()->Set(ControlMode::PercentOutput, maxSpeed/*(m_Y + m_X + m_A)/max*maxSpeed*/ );
	//Robot::Get().GetDriveTrain().GetRFront()->Set(ControlMode::PercentOutput, maxSpeed/*(m_Y - m_X - m_A)/max*maxSpeed*/ );
	//Robot::Get().GetDriveTrain().GetLBack()->Set(ControlMode::PercentOutput, maxSpeed/*(m_Y - m_X + m_A)/max*maxSpeed*/ );
	//Robot::Get().GetDriveTrain().GetRBack()->Set(ControlMode::PercentOutput, maxSpeed/*(m_Y + m_X - m_A)/max*maxSpeed*/ );

	//Robot::Get().GetDriveTrain().GetLFront()->Set(ControlMode::Velocity, (m_Y + m_X + m_A)/max*maxSpeed*DriveTrain::kMAX_VELOCITY );
	//Robot::Get().GetDriveTrain().GetRFront()->Set(ControlMode::Velocity, (m_Y - m_X - m_A)/max*maxSpeed*DriveTrain::kMAX_VELOCITY );
	//Robot::Get().GetDriveTrain().GetLBack()->Set(ControlMode::Velocity, (m_Y - m_X + m_A)/max*maxSpeed*DriveTrain::kMAX_VELOCITY );
	//Robot::Get().GetDriveTrain().GetRBack()->Set(ControlMode::Velocity, (m_Y + m_X - m_A)/max*maxSpeed*DriveTrain::kMAX_VELOCITY );

	//End graveyard

	Robot::Get().GetDriveTrain().GetLFront()->Set(ControlMode::Position, m_InitialTicks[0]+(m_Y + m_X + m_A) );
	Robot::Get().GetDriveTrain().GetRFront()->Set(ControlMode::Position, m_InitialTicks[1]+(m_Y - m_X - m_A) );
	Robot::Get().GetDriveTrain().GetLBack()->Set(ControlMode::Position,  m_InitialTicks[2]+(m_Y - m_X + m_A) );
	Robot::Get().GetDriveTrain().GetRBack()->Set(ControlMode::Position,  m_InitialTicks[3]+(m_Y + m_X - m_A) );

}//execute command (does nothing, waits)

void EncoderDrive::End(bool interrupted) {
	
	DebugOutF("|STOP|");
	DebugOutF("Final Pos diff: "+std::to_string( Robot::Get().GetDriveTrain().GetLFront()->GetSelectedSensorPosition() - (m_Y + m_A + m_InitialTicks[0] + m_X) ));

	
	Robot::Get().GetDriveTrain().GetLFront()->SetNeutralMode(Brake);
	Robot::Get().GetDriveTrain().GetRFront()->SetNeutralMode(Brake);
	Robot::Get().GetDriveTrain().GetLBack()->SetNeutralMode(Brake);
	Robot::Get().GetDriveTrain().GetRBack()->SetNeutralMode(Brake);

	Robot::Get().GetDriveTrain().UseVelocityPID();

}//stops motors and exits
//end override commands

frc2::Command* EncoderDrive::RotateTo(double angle){
	if (abs(angle) < 10){
		std::vector<std::unique_ptr<frc2::Command>> vector;
		if (angle < 0){
			vector.push_back( std::make_unique<EncoderDrive>(0, 0, 10));
			vector.push_back( std::make_unique<EncoderDrive>(0, 0, angle - 10));
		}
		else {
			vector.push_back( std::make_unique<EncoderDrive>(0, 0, -10));
			vector.push_back( std::make_unique<EncoderDrive>(0, 0, angle + 10));
		}
		return new frc2::SequentialCommandGroup(std::move(vector));
	} else if (abs(angle) < 40) {
		return new EncoderDrive(0, 0, angle);
	} else {
		return Robot::Get().GetDriveTrain().TurnToPos(angle);
	}
}
//end EncoderDrive

}//end namespace