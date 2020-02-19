#include "OI.h"
#include "Util.h"
#include "Cob.h"
#include "Robot.h"
#include "commands/EncoderDrive.h"
#include "commands/PlayMusic.h"

namespace ohs2020{

OI::OI() : 

m_FodToggle([&] { return ButtonID FOD_TOGGLE_ID;}),
m_Turn([&] { return JoystickID TURN_ID;})
{

}


void OI::Init() {

	m_FodToggle.WhenPressed(frc2::InstantCommand([&] { 

		m_Fod = !m_Fod;
		
		DebugOutF("Flipped FOD");
		
	}, {} ));

	m_Turn.WhenPressed( EncoderDrive::RotateTo(30) );
	m_Music.WhenPressed( new PlayMusic() );

	m_VModeToggle.WhenPressed( []() {DebugOutF( "VMODE?: " + std::to_string( Robot::Get().GetOI().ToggleVMode()) ); } );
}



}//namespace