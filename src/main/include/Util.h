#pragma once

#include <frc2/command/PrintCommand.h>
#include <frc2/command/CommandScheduler.h>
#include <frc2/command/WaitCommand.h>

#include <iostream>
#include <chrono>
#include <thread>

inline void DebugOutF(const wpi::Twine& message){
	frc2::CommandScheduler::GetInstance().Schedule(new frc2::PrintCommand(message));
}

inline void Wait(int ms) {

	frc2::CommandScheduler::GetInstance().Schedule(new frc2::WaitCommand(std::chrono::milliseconds(ms)));

}