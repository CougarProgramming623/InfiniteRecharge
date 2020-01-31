#include "OI.h"
#include "Util.h"

namespace ohs2020{

OI::OI() : 

m_FodToggle([&] { return m_DriverJoystick.GetRawButton(1);}){

}


void OI::Init(){

}

inline void OI::FlipFOD(){

	m_Fod = !m_Fod;

}

void OI::FlipFOD(){

	m_Fod = !m_Fod;

	DebugOutF("Flipped FOD to: " + std::to_string(m_Fod));

}

}//namespace