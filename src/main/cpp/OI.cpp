#include "OI.h"
#include "Util.h"
#include "Cob.h"
#include "Robot.h"
#include "commands/EncoderDrive.h"
#include "ButtonIDs.h"

namespace ohs2020 {

OI::OI() : 

m_FodToggle(m_DriverJoystick, FOD_TOGGLE_ID),
m_Turn(m_ButtonBoard, TURN_ID),
m_VModeToggle(m_ButtonBoard, 16)//FIXME
{

}


void OI::Init() {

	m_FodToggle.WhenPressed(frc2::InstantCommand([&] { 

		m_Fod = !m_Fod;
		
		DebugOutF("Flipped FOD");
		
	}, {} ));

	m_Turn.WhenPressed( EncoderDrive::RotateTo(30) );

	m_VModeToggle.WhenPressed( []() {DebugOutF( "VMODE?: " + std::to_string( Robot::Get().GetOI().ToggleVMode()) ); } );
}



}//namespace