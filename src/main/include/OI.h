#pragma once

#include "ohs/Button.h"
#include "commands/DriveWithJoysticks.h"

namespace ohs2020 {

class OI { 

public:
	OI();
		
    void Init();

	frc::Joystick& GetDriverJoystick() { return m_DriverJoystick; }
	frc::Joystick& GetButtonBoard() { return m_ButtonBoard; }

	bool IsFOD() const { return m_Fod; }

	bool GetVelocityMode() const { return m_VelocityMode; } 

	bool isFodToggle() const { return m_FodToggle.Get(); }

	bool ToggleVMode() {m_VelocityMode = !m_VelocityMode; return m_VelocityMode;}

	ohs623::Button m_FodToggle;
	ohs623::Button m_Turn;
	ohs623::Button m_VModeToggle;
	
	bool m_Fod = true;

private:
	frc::Joystick m_DriverJoystick = frc::Joystick(0);
	frc::Joystick m_ButtonBoard = frc::Joystick(1);

	bool m_VelocityMode = false;

};

}//namespace