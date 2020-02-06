#include "OI.h"
#include "Util.h"
#include "Robot.h"

namespace ohs2020{

OI::OI() : 

m_FodToggle([&] { return m_DriverJoystick.GetRawButton(1);}),
m_Turn([&] { return m_ButtonBoard.GetRawButton(19);})
{

}


void OI::Init(){

	m_FodToggle.WhenPressed(frc2::InstantCommand([&] { 
		m_Fod = !m_Fod;
		DebugOutF("Flipped FOD");
	}, {} ));


	m_Turn.WhenPressed(Robot::Get().GetDriveTrain().TurnToPos(90));
}


void OI::FlipFOD(){

	m_Fod = !m_Fod;

	DebugOutF("Flipped FOD to: " + std::to_string(m_Fod));

}

}//namespace