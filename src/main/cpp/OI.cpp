#include "OI.h"

namespace ohs2020{

OI::OI() :
m_DriverJoystick(0), m_ButtonBoard(1) {
	m_VelocityMode = true;
	m_Fod = true;
}

}//namespace