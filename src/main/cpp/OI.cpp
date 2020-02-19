#include "OI.h"
#include "Util.h"
#include "Cob.h"
#include "Robot.h"
#include "commands/EncoderDrive.h"
#include "ButtonIDs.h"

namespace ohs2020 {

OI::OI() : 

m_FodToggle([&] { return m_DriverJoystick.GetRawButton(1);}),
m_Turn([&] { return m_ButtonBoard.GetRawButton(19);}),
m_EncoderDrive([&] { return m_ButtonBoard.GetRawButton(2);})
{

}


void OI::Init() {

	m_FodToggle.WhenPressed(frc2::InstantCommand([&] { 

		m_Fod = !m_Fod;
		
		DebugOutF("Flipped FOD");
		
	}, {} ));

	m_Turn.WhenPressed( EncoderDrive::RotateTo(30) );

	m_VModeToggle.WhenPressed( []() {DebugOutF( "VMODE?: " + std::to_string( Robot::Get().GetOI().ToggleVMode()) ); } );

	m_EncoderDrive.WhenPressed( new EncoderDrive(0, 0, 50));
}



}//namespace