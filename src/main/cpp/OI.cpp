#include "OI.h"
#include "Util.h"

namespace ohs2020{

OI::OI() : m_FodToggle(&m_DriverJoystick, 1) {
	m_VelocityMode = true;

	m_FodToggle.WhenPressed(new ToggleBoolean(&m_Fod, [](bool newValue){DebugOutF("Current Driver Orientation State: " + newValue ? "true" : "false");}), false);
}

void OI::Init(){
    OI();
}

}//namespace