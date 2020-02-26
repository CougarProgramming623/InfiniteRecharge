#include "OI.h"
#include "Util.h"
#include "Cob.h"
#include "Robot.h"
#include "commands/EncoderDrive.h"
#include "commands/TurnToPosPID.h"

namespace ohs2020{

OI::OI() : 

m_FodToggle([&] { return m_DriverJoystick.GetRawButton(1);}),
m_Turn([&] { return m_ButtonBoard.GetRawButton(1);}),
m_VModeToggle([&] { return m_DriverJoystick.GetRawButton(2);}),
m_LimelightToggle([&] { return m_ButtonBoard.GetRawButton(14);}) 
{

}


void OI::Init(){

	m_FodToggle.WhenPressed(frc2::InstantCommand([&] { 

		m_Fod = !m_Fod;
		
		DebugOutF("Flipped FOD");
		
	}, {} ));

	m_LimelightToggle.WhenPressed(frc2::InstantCommand([&] { 
		Cob::PushValue(CobKey::LIMELIGHT_TOGGLE, 3); 
	}, {} ));

	m_LimelightToggle.WhenReleased(frc2::InstantCommand([&] { 
		Cob::PushValue(CobKey::LIMELIGHT_TOGGLE, 1); 
	}, {} ));

	m_Turn.WhenPressed( new TurnToPosPID() );

}



}//namespace