#pragma once

#include <frc2/command/button/JoystickButton.h>
#include <frc/Joystick.h>

namespace ohs2020 {

class OI { 

public:
    OI();
    

    frc::Joystick& GetDriverJoystick() { return m_DriverJoystick; }
    frc::Joystick& GetButtonBoard() { return m_ButtonBoard; }

    bool IsFOD() const { return m_Fod; }

    bool GetVelocityMode() const { return m_VelocityMode; } 

private:
    frc::Joystick m_DriverJoystick;
    frc::Joystick m_ButtonBoard;

    bool m_Fod;
    bool m_VelocityMode;

};

}