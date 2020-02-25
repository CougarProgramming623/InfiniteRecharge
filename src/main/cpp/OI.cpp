#include "OI.h"
#include "Util.h"
#include "Cob.h"
#include "Robot.h"
#include "commands/EncoderDrive.h"
#include "commands/EncoderDriveV.h"
#include <frc2/command/SequentialCommandGroup.h>

namespace ohs2020{

OI::OI() : 

m_FodToggle([&] { return m_DriverJoystick.GetRawButton(1); }),
m_Turn([&] { return m_ButtonBoard.GetRawButton(1); }),
m_EncoderDrive([&] { return m_ButtonBoard.GetRawButton(14); })
{

}


void OI::Init(){

	m_FodToggle.WhenPressed(frc2::InstantCommand([&] { 
		m_Fod = !m_Fod;
		Cob::SendMessage(CobMessageOut::PING, "Is Field Oriented");
		DebugOutF("Flipped FOD");
	}, {} ));

	//m_Turn.WhenPressed( Robot::Get().GetDriveTrain().TurnToPos(90) );


	/*
	frc2::InstantCommand inst = frc2::InstantCommand([&] { 
		EncoderDrive(0, 24576, 0);
		Robot::Get().GetDriveTrain().GetLFront()->SetNeutralMode(Brake);
		Robot::Get().GetDriveTrain().GetRFront()->SetNeutralMode(Brake);
		Robot::Get().GetDriveTrain().GetLBack()->SetNeutralMode(Brake);
		Robot::Get().GetDriveTrain().GetRBack()->SetNeutralMode(Brake);
	}, {} );
	*/
	//std::vector<std::unique_ptr<frc2::Command>> vector{std::make_unique<EncoderDrive>(0, 24576, 0), std::make_unique<frc2::InstantCommand>(inst) };

	// vector.push_back(std::make_unique<EncoderDrive>(0, 24576, 0) );
	// vector.push_back(std::make_unique<frc2::InstantCommand>(inst) );
	//new frc2::SequentialCommandGroup(std::move(vector)
	m_EncoderDrive.WhenPressed( new EncoderDriveV(36.0, 0.0, 0) );
}



}//namespace