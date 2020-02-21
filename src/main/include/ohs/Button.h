#pragma once

#include <frc2/command/button/JoystickButton.h>
#include <frc/Joystick.h>

#define OHS_BUTTON(id) []() { return Robot::Get().GetOI().GetDriverJoystick().GetRawButton(id); }
#define OHS_DRIVER_BUTTON(id) [](){ return Robot::Get().GetOI().GetDriverJoystick().GetRawButton(id); }

