#include "subsystems/Climb.h"
#include "Robot.h"
#include "ohs/RobotID.h"
#include "Util.h"

#include <frc2/Timer.h>
#include <frc2/command/SequentialCommandGroup.h>

using namespace ohs623;

namespace ohs2020 {

double BASIC_CLIMB_SPEED = 1;

Climb::Climb() : 


m_ClimbMotorLeft		( RobotID::GetID(CLIMB_LEFT)),
m_ClimbMotorRight		( RobotID::GetID(CLIMB_RIGHT)), 

m_ClimbUp([&]        	{ return Robot::Get().GetOI().GetButtonBoard().GetRawButton(19);  }),
m_ClimbDown([&]			{ return Robot::Get().GetOI().GetButtonBoard().GetRawButton(20);  }),
m_ClimbLeft([&]       	{ return Robot::Get().GetOI().GetButtonBoard().GetRawButton(21);  }),
m_ClimbRight([&]      	{ return Robot::Get().GetOI().GetButtonBoard().GetRawButton(22);  }),
m_Deployer([&] 			{ return Robot::Get().GetOI().GetButtonBoard().GetRawButton(17);  }),
m_EndgameOverride([&] 	{ return Robot::Get().GetOI().GetButtonBoard().GetRawButton(18);  }),

m_Timer()
{


	m_ClimbMotorLeft.SetInverted(true);
	m_ClimbMotorRight.SetInverted(false);

	m_ClimbMotorLeft.SetNeutralMode(Brake);
	m_ClimbMotorRight.SetNeutralMode(Brake);

	RemoveRegistry(&m_ClimbMotorLeft, &m_ClimbMotorRight);
  
}

void Climb::Init() {

isDeployed = false;
isDeployFinished = false;

VerticalClimb();
SideClimb();
Deploy();

}

void Climb::VerticalClimb() {

m_ClimbUp.WhileHeld(frc2::RunCommand([&] {

	if(CanClimb()){
		m_ClimbMotorLeft.Set(ControlMode::PercentOutput, -BASIC_CLIMB_SPEED);//correct direction
		m_ClimbMotorRight.Set(ControlMode::PercentOutput, -BASIC_CLIMB_SPEED);

		DebugOutF("Climbing Up");
	}

 }, {} ));

 m_ClimbUp.WhenReleased(frc2::InstantCommand([&] {

	m_ClimbMotorLeft.Set(ControlMode::PercentOutput, 0);
	m_ClimbMotorRight.Set(ControlMode::PercentOutput, 0);

 }, {} ));

 m_ClimbDown.WhenHeld(frc2::RunCommand([&] {

	if(CanClimb()){
		m_ClimbMotorLeft.Set(ControlMode::PercentOutput, BASIC_CLIMB_SPEED);
		m_ClimbMotorRight.Set(ControlMode::PercentOutput, BASIC_CLIMB_SPEED);//correct direction

		DebugOutF("Climbing Down");
	}

 }, {} ));

 m_ClimbDown.WhenReleased(frc2::InstantCommand([&] {

	m_ClimbMotorLeft.Set(ControlMode::PercentOutput, 0);
	m_ClimbMotorRight.Set(ControlMode::PercentOutput, 0);

 }, {} ));
}


void Climb::SideClimb() {

m_ClimbLeft.WhenHeld(frc2::RunCommand([&] {

	if(CanClimb()){
		m_ClimbMotorLeft.Set(ControlMode::PercentOutput, BASIC_CLIMB_SPEED);
		m_ClimbMotorRight.Set(ControlMode::PercentOutput, -BASIC_CLIMB_SPEED);

		DebugOutF("Climbing Left");

	}

}, {} ));

m_ClimbLeft.WhenReleased(frc2::InstantCommand([&] {

	m_ClimbMotorLeft.Set(ControlMode::PercentOutput, 0);
	m_ClimbMotorRight.Set(ControlMode::PercentOutput, 0);

}, {} ));


m_ClimbRight.WhenHeld(frc2::RunCommand([&] {
	
	if(CanClimb()){
		
		m_ClimbMotorLeft.Set(ControlMode::PercentOutput, -BASIC_CLIMB_SPEED);
		m_ClimbMotorRight.Set(ControlMode::PercentOutput, BASIC_CLIMB_SPEED);

		DebugOutF("Climbing Right");
	}

 }, {} ));


m_ClimbRight.WhenReleased(frc2::InstantCommand([&] {

	m_ClimbMotorLeft.Set(ControlMode::PercentOutput, 0);
	m_ClimbMotorRight.Set(ControlMode::PercentOutput, 0);

}, {} ));

}

void Climb::Deploy() {

	m_Deployer.WhenPressed(frc2::FunctionalCommand( [this] { //on init
     
		if(!isDeployed && IsEndgame()){
			DebugOutF("start deploy");
			m_Timer.Reset();
			m_Timer.Start();
			m_ClimbMotorLeft.Set(ControlMode::PercentOutput, 1); //correct direction
			m_ClimbMotorRight.Set(ControlMode::PercentOutput, -1);

			frc2::CommandScheduler::GetInstance().Schedule(new frc2::SequentialCommandGroup(
				frc2::WaitCommand(units::second_t(5)),
				frc2::InstantCommand( [&] { isDeployFinished = true; }, 
			{} )));
		}

		DebugOutF("Deploying **");

	}, [this] {}, [this] (bool f) { //on end

		if(IsEndgame()){
			m_ClimbMotorLeft.Set(ControlMode::PercentOutput, 0);
			m_ClimbMotorRight.Set(ControlMode::PercentOutput, 0);
			isDeployed = true;
		}
			

	}, [this] { //is finished

		if(isDeployed)
			return true;
		if(!IsEndgame()) 
			return true;

		DebugOutF("Deploying");

		return m_Timer.Get() > units::second_t(.2); //Time to trigger gas spring

	}, {}));

}

void Climb::LEDCanClimb() {
	if(CanClimb()){
		Robot::Get().GetOI().GetButtonBoard().SetOutput(7, true);
	} else{
		Robot::Get().GetOI().GetButtonBoard().SetOutput(7, false);

	}
}

inline bool Climb::IsEndgame() {
	return (Timer::GetMatchTime() < 30 && Timer::GetMatchTime() != -1) ||  m_EndgameOverride.Get();
}

inline bool Climb::CanClimb() {
	return ((Timer::GetMatchTime() < 30 && Timer::GetMatchTime() != -1 ) && isDeployFinished) || m_EndgameOverride.Get();
}

}//namespace