#pragma once

#include <frc2/command/PrintCommand.h>
#include <frc2/command/CommandScheduler.h>
#include <frc2/command/WaitCommand.h>
#include <frc2/command/SequentialCommandGroup.h>

#include <frc/smartdashboard/Sendable.h>

#include <iostream>
#include <chrono>
#include <thread>

inline void DebugOutF(const wpi::Twine& message){
	frc2::CommandScheduler::GetInstance().Schedule(new frc2::PrintCommand(message));
}

template <typename... Types>
void RemoveRegistry(Types... nextSendables) {}

template <typename T, typename... Types>
void RemoveRegistry(T* currentSendable, Types... nextSendables) {

	frc::Sendable* sendableptr = dynamic_cast<frc::Sendable*>(currentSendable);

	if (sendableptr == nullptr) return;
	
	frc::SendableRegistry::GetInstance().Remove(sendableptr);

	RemoveRegistry(nextSendables...);
}



