#include "OI.h"
#include "Util.h"

namespace ohs2020{

OI::OI() : 

m_FodToggle([&] { return m_DriverJoystick.GetRawButton(1);}){

}


void OI::Init(){

}

void OI::FlipFOD(){

	m_Fod = !m_Fod;

}

}//namespace