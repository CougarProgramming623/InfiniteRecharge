#include "OI.h"
#include "Util.h"
#include "Cob.h"
#include "Robot.h"
#include "commands/EncoderDrive.h"
#include "commands/PlayMusic.h"

namespace ohs2020{

OI::OI() : 

m_FodToggle([&] { return m_DriverJoystick.GetRawButton(1); }),
m_Turn([&] { return m_ButtonBoard.GetRawButton(100); }),
m_Music([&] { return m_ButtonBoard.GetRawButton(24); })
{

}


void OI::Init(){

	m_FodToggle.WhenPressed(frc2::InstantCommand([&] { 
		m_Fod = !m_Fod;
		Cob::SendMessage(CobMessageOut::PING, "Is Field Oriented");
		DebugOutF("Flipped FOD");
	}, {} ));

	m_Turn.WhenPressed( EncoderDrive::RotateTo(30) );
	m_Music.WhenPressed( new PlayMusic() );
}



}//namespace