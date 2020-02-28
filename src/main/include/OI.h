#pragma once

#include <frc2/command/button/JoystickButton.h>
#include <frc/Joystick.h>

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

	frc2::Button m_FodToggle;
	frc2::Button m_Turn;
	frc2::Button m_EncoderDrive;
	frc2::Button m_LimelightToggle;
	frc2::Button m_TestShoot;
		
	bool m_Fod = true;

private:
	frc::Joystick m_DriverJoystick = frc::Joystick(0);
	frc::Joystick m_ButtonBoard = frc::Joystick(1);

	bool m_VelocityMode = true;

};

}//namespace