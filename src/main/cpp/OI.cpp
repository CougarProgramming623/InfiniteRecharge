#include "OI.h"
#include "Util.h"
#include "Robot.h"
#include "commands/EncoderDrive.h"

namespace ohs2020{

OI::OI() : 

m_FodToggle([&] { return m_DriverJoystick.GetRawButton(1);}),
m_Turn([&] { return m_ButtonBoard.GetRawButton(19);}),
m_EncoderDrive([&] {return m_ButtonBoard.GetRawButton(2);} )
{

}


void OI::Init(){

	m_FodToggle.WhenPressed(frc2::InstantCommand([&] { 
		m_Fod = !m_Fod;
		DebugOutF("Flipped FOD");
	}, {} ));



	m_Turn.WhenPressed(Robot::Get().GetDriveTrain().TurnToPos(90));

	m_EncoderDrive.WhenPressed( new EncoderDrive(0, 2048*12, 0.0) );
}


void OI::FlipFOD(){

	m_Fod = !m_Fod;

	DebugOutF("Flipped FOD to: " + std::to_string(m_Fod));

}

}//namespace