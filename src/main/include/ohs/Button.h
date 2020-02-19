#pragma once

#include <frc2/command/button/JoystickButton.h>
#include <frc/Joystick.h>

//A fix for the grabage class frc2::JoystickButton during the 2020 season
namespace ohs623 {

class Button : public frc2::Button {
public:
	Button(frc::Joystick& stick, int id) : frc2::Button([&] { return stick.GetRawButton(id); }) {}
};


}