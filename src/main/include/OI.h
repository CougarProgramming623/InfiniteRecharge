#pragma once

#include <frc2/command/button/JoystickButton.h>
#include <frc/Joystick.h>

#include "commands/DriveWithJoysticks.h"
#include "commands/ToggleBoolean.h"

namespace ohs2020 {

class OI { 

public:
	OI();
		
    void Init();

	frc::Joystick& GetDriverJoystick() { return m_DriverJoystick; }
	frc::Joystick& GetButtonBoard() { return m_ButtonBoard; }

	bool IsFOD() const { return m_Fod; }

	bool GetVelocityMode() const { return m_VelocityMode; } 

private:
	frc::Joystick m_DriverJoystick = frc::Joystick(0);
	frc::Joystick m_ButtonBoard = frc::Joystick(1);

	bool m_Fod = true;
	bool m_VelocityMode = true;

	frc2::JoystickButton m_FodToggle;
};

}//namespace