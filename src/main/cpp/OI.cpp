#include "OI.h"
#include "Util.h"

namespace ohs2020{

OI::OI() : m_FodToggle(&m_DriverJoystick, 1), b1(&m_ButtonBoard, 2), b2(&m_ButtonBoard, 3), b3(&m_ButtonBoard, 4),
		b4(&m_ButtonBoard, 5), b5(&m_ButtonBoard, 6), b6(&m_ButtonBoard, 7), b7(&m_ButtonBoard, 8) {
	

}

void OI::Init(){

    m_FodToggle.WhenPressed(new frc2::PrintCommand(std::to_string(isFodToggle())), false);

	b1.WhenPressed(new frc2::PrintCommand(std::to_string(isFodToggle())), false);
	b2.WhenPressed(new frc2::PrintCommand(std::to_string(isFodToggle())), false);
	b3.WhenPressed(new frc2::PrintCommand(std::to_string(isFodToggle())), false);
	b4.WhenPressed(new frc2::PrintCommand(std::to_string(isFodToggle())), false);
	b5.WhenPressed(new frc2::PrintCommand(std::to_string(isFodToggle())), false);
	b6.WhenPressed(new frc2::PrintCommand(std::to_string(isFodToggle())), false);
	b7.WhenPressed(new frc2::PrintCommand(std::to_string(isFodToggle())), false);
	


	DebugOutF("Ran OI Init");

	//m_FodToggle.WhenPressed(new ToggleBoolean(&m_Fod, [](bool newValue){DebugOutF("Current Driver Orientation State: " + newValue ? "true" : "false");}), false);
}

}//namespace