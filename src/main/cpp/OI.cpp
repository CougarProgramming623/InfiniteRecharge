#include "OI.h"
#include "Util.h"
#include "Cob.h"
#include "Robot.h"
#include "commands/EncoderDrive.h"
#include "commands/TurnToPosPID.h"

namespace ohs2020{

OI::OI() : 

m_FodToggle([&] { return m_DriverJoystick.GetRawButton(1);}),
m_Turn([&] { return m_ButtonBoard.GetRawButton(19);}),
m_EncoderDrive([&] { return m_ButtonBoard.GetRawButton(2);})
{

}


void OI::Init(){

	m_FodToggle.WhenPressed(frc2::InstantCommand([&] { 
		m_Fod = !m_Fod;
		Cob::SendMessage(CobMessageOut::PING, "Is Field Oriented");
		DebugOutF("Flipped FOD");
	}, {} ));

	m_Turn.WhenPressed( new TurnToPosPID(30) );

	m_EncoderDrive.WhenPressed( new EncoderDrive(0, 0, 50));
}



}//namespace