#include "OI.h"
#include "Util.h"
#include "Cob.h"
#include "Robot.h"
#include "commands/EncoderDrive.h"
#include "commands/EncoderDriveV.h"
#include "commands/EncoderDriveM.h"
#include "commands/TurnToPosPID.h"
#include "commands/TurnToPosSlow.h"

namespace ohs2020{

OI::OI() : 

m_FodToggle([&] { return m_DriverJoystick.GetRawButton(1);}),
m_Turn([&] { return m_DriverJoystick.GetRawButton(14);}),
m_LimelightToggle([&] { return m_ButtonBoard.GetRawButton(3);}) // 1 off 3 on
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

	m_Turn.WhenPressed( new EncoderDriveV(0.0, 36.0, 0) );

}



}//namespace