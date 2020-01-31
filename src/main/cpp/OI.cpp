#include "OI.h"
#include "Util.h"
#include "Cob.h"

namespace ohs2020{

OI::OI() : 

m_FodToggle([&] { return m_DriverJoystick.GetRawButton(1);}){

}


void OI::Init(){

	m_FodToggle.WhenPressed(frc2::InstantCommand([&] { 
		m_Fod = !m_Fod;
		Cob::SendMessage(CobMessageOut::PING, "Is Field Oriented");
		DebugOutF("Flipped FOD");
	}, {} ));

}


void OI::FlipFOD(){

	m_Fod = !m_Fod;

	DebugOutF("Flipped FOD to: " + std::to_string(m_Fod));

}

}//namespace