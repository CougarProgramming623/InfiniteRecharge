#include "AutoManager.h"
#include "commands/EncoderDriveV.h"
#include "commands/TurnToPosSlow.h"

#include <vector>
#include <memory>

namespace ohs2020{

AutoManager::AutoManager(){}
AutoManager::~AutoManager(){
	for (std::pair<std::string, frc2::Command*> element : m_AutoMap) {
		delete element.second;
	}
}

void AutoManager::AutoInit(){
	m_AutoMap["default"] = new frc2::SequentialCommandGroup(frc2::PrintCommand("Init"), frc2::WaitCommand(units::second_t(m_Delay)), frc2::PrintCommand("Move"), frc2::PrintCommand("Shoot"));
	m_AutoMap["fancy"] = new frc2::SequentialCommandGroup(frc2::WaitCommand(units::second_t(m_Delay)), frc2::PrintCommand("Move"), frc2::PrintCommand("Aligned"), frc2::PrintCommand("Shoot"));
	m_AutoMap["safe"] = new frc2::SequentialCommandGroup(frc2::WaitCommand(units::second_t(m_Delay)), EncoderDriveV(0.0, -24.0, 0));

	m_AutoMap["noop"] = new frc2::SequentialCommandGroup(frc2::PrintCommand("Init"), frc2::WaitCommand(units::second_t(m_Delay)), frc2::PrintCommand("Did Nothing"));
	m_AutoMap["shoot-forward"] = new frc2::SequentialCommandGroup(frc2::PrintCommand("Init"), frc2::WaitCommand(units::second_t(m_Delay)), TurnToPosSlow(), frc2::PrintCommand("SHOOT"), frc2::PrintCommand("SHOOT"), frc2::PrintCommand("SHOOT"), EncoderDriveV(0.0, 24.0, 0) );
	m_AutoMap["shoot-backward"] = new frc2::SequentialCommandGroup(frc2::PrintCommand("Init"), frc2::WaitCommand(units::second_t(m_Delay)), TurnToPosSlow(), frc2::PrintCommand("SHOOT"), frc2::PrintCommand("SHOOT"), frc2::PrintCommand("SHOOT"), EncoderDriveV(0.0, -24.0, 0));
	//m_AutoMap["debug"] = new frc2::SequentialCommandGroup(frc2::WaitCommand(units::second_t(m_Delay)), frc2::InstantCommand([&]{OHS_DEBUG([&](auto f){f << "Debugged";})}));
}

void AutoManager::SetInUse(std::string setAuto) {
	if (setAuto != "--DELETED--") {
		m_InUse = setAuto;
	}
}

frc2::Command* AutoManager::GetAuto() {
	std::vector<std::unique_ptr<frc2::Command>> steps;
	steps.emplace_back(new frc2::WaitCommand(units::second_t(m_Delay)));
	steps.emplace_back(m_AutoMap[m_InUse]);

	return new frc2::SequentialCommandGroup(std::move(steps));
}

void AutoManager::RunAuto(){
	if (m_AutoMap.find(m_InUse) != m_AutoMap.end()) {
		frc2::CommandScheduler::GetInstance().Schedule(m_AutoMap[m_InUse]);
	} else {
		OHS_ERROR([&](auto& f) {
			f << "Name not in map: " << m_InUse << "\nMap is: ";
			for (std::map<std::string, frc2::Command*>::iterator it = m_AutoMap.begin(); it != m_AutoMap.end(); it++) {
				f << it->first << " = " << it->second->GetName();
			}
		});
	}
	
}
}