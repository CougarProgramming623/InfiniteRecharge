#include "OI.h"
#include "Util.h"

namespace ohs2020{

OI::OI() : 

m_FodToggle([&] { return m_DriverJoystick.GetRawButton(1);}){

}


void OI::Init(){

<<<<<<< HEAD
}

void OI::FlipFOD(){
=======
	

/*
	b1.WhenPressed(new frc2::PrintCommand(std::to_string(isFodToggle())));
	b2.WhenPressed(new frc2::PrintCommand(std::to_string(isFodToggle())));
	b3.WhenPressed(new frc2::PrintCommand(std::to_string(isFodToggle())));
	b4.WhenPressed(new frc2::PrintCommand(std::to_string(isFodToggle())));
	b5.WhenPressed(new frc2::PrintCommand(std::to_string(isFodToggle())));
	b6.WhenPressed(new frc2::PrintCommand(std::to_string(isFodToggle())));
	b7.WhenPressed(new frc2::PrintCommand(std::to_string(isFodToggle())));
	b8.WhenPressed(new frc2::PrintCommand(std::to_string(isFodToggle())));
	b9.WhenPressed(new frc2::PrintCommand(std::to_string(isFodToggle())));
*/
>>>>>>> drive-train-carson

	m_Fod = !m_Fod;

}

void OI::FlipFOD(){

	m_Fod = !m_Fod;

	DebugOutF("Flipped FOD to: " + std::to_string(m_Fod));

}

}//namespace