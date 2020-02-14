#pragma once

#include <frc2/command/PrintCommand.h>
#include <frc2/command/CommandScheduler.h>
#include <frc2/command/WaitCommand.h>
#include <frc2/command/SequentialCommandGroup.h>

#include <iostream>
#include <chrono>
#include <thread>

inline void DebugOutF(const wpi::Twine& message){
	frc2::CommandScheduler::GetInstance().Schedule(new frc2::PrintCommand(message));
}