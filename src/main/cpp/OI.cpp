#include "OI.h"
#include "Util.h"
#include "Cob.h"
#include "Robot.h"
#include "commands/EncoderDrive.h"
#include "commands/TurnToPosPID.h"

namespace ohs2020{

OI::OI() : 

m_FodToggle([&] { return m_DriverJoystick.GetRawButton(1);}),
m_Turn([&] { return m_ButtonBoard.GetRawButton(1);})
//m_EncoderDrive([&] { return m_ButtonBoard.GetRawButton(2);}),
//m_VModeToggle([&] { return m_ButtonBoard.GetRawButton(31);})
{

}


void OI::Init(){

	m_FodToggle.WhenPressed(frc2::InstantCommand([&] { 

		m_Fod = !m_Fod;
		
		DebugOutF("Flipped FOD");
		
	}, {} ));

	m_Turn.WhileHeld( new TurnToPosPID() );

	//m_EncoderDrive.WhenPressed( new EncoderDrive(0, 0, 50));
}



}//namespace