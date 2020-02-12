#pragma once

#include <frc2/command/PrintCommand.h>
#include <frc2/command/CommandScheduler.h>
#include <frc2/Timer.h>

inline void DebugOutF(const wpi::Twine& message){
	frc2::CommandScheduler::GetInstance().Schedule(new frc2::PrintCommand(message));
}

inline void Wait(double milliseconds) {

	frc2::Timer timer;

	timer.Reset();
	timer.Start();

	while(static_cast <double> (timer.Get()) * 1000 < milliseconds);

}