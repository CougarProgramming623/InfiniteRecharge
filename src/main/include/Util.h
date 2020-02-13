#pragma once

#include <frc2/command/PrintCommand.h>
#include <frc2/command/CommandScheduler.h>
#include <frc2/command/WaitCommand.h>

#include <iostream>
#include <chrono>
#include <ratio>
#include <thread>

inline void DebugOutF(const wpi::Twine& message){
	frc2::CommandScheduler::GetInstance().Schedule(new frc2::PrintCommand(message));
}

inline void Wait(int ms) {

	std::this_thread::sleep_for(std::chrono::milliseconds(ms));

}