#pragma once

#include <frc2/command/PrintCommand.h>

#include <frc2/command/CommandScheduler.h>

inline void DebugOutF(const wpi::Twine& message){
	frc2::CommandScheduler::GetInstance().Schedule(new frc2::PrintCommand(message));
}