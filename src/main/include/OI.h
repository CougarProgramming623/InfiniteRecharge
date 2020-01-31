#pragma once

#include <frc2/command/button/JoystickButton.h>
#include <frc/Joystick.h>

#include "commands/DriveWithJoysticks.h"

namespace ohs2020 {

class OI { 

public:
	OI();
		
    void Init();
	void FlipFOD();

	frc::Joystick& GetDriverJoystick() { return m_DriverJoystick; }
	frc::Joystick& GetButtonBoard() { return m_ButtonBoard; }

	bool IsFOD() const { return m_Fod; }

	bool GetVelocityMode() const { return m_VelocityMode; } 

	bool isFodToggle() const { return m_FodToggle.Get(); }
	
	frc2::Button m_FodToggle;
	bool m_Fod = true;

private:
	frc::Joystick m_DriverJoystick = frc::Joystick(0);
	frc::Joystick m_ButtonBoard = frc::Joystick(1);

	bool m_VelocityMode = false;

	

	// frc2::Button b1;
	// frc2::Button b2;
	// frc2::Button b2;
	// frc2::Button b2;
	// frc2::Button b2;
	// frc2::Button b2;
	// frc2::Button b2;
	// frc2::Button b2;
	// frc2::Button b2;
	// frc2::Button b2;
	// frc2::Button b2;
	// frc2::Button b3;
	// frc2::Button b4;
	// frc2::Button b5;
	// frc2::Button b6;
	// frc2::Button b7;
	// frc2::Button b8;
	// frc2::Button b9;
};

}//namespace