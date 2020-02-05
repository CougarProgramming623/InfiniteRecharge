#include "OI.h"
#include "Util.h"

namespace ohs2020{

OI::OI() : 

m_FodToggle([&] { return m_DriverJoystick.GetRawButton(1);}),
TESTBUTTONDELETE([&] { return m_ButtonBoard.GetRawButton(19);})
{

}


void OI::Init(){

	m_FodToggle.WhenPressed(frc2::InstantCommand([&] { 
		m_Fod = !m_Fod;
		DebugOutF("Flipped FOD");
	}, {} ));

	TESTBUTTONDELETE.WhenPressed(frc2::InstantCommand([&] { 
		
		DebugOutF("Pressed");
	}, {} ));

}


void OI::FlipFOD(){

	m_Fod = !m_Fod;

	DebugOutF("Flipped FOD to: " + std::to_string(m_Fod));

}

}//namespace