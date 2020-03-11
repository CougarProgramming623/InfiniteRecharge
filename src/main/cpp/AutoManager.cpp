#include "AutoManager.h"
#include "commands/EncoderDriveV.h"
#include "commands/EncoderDriveM.h"
#include "commands/TurnToPosSlow.h"
#include "commands/TurnToPosPID.h"
#include "subsystems/Shooter.h"
#include "Robot.h"

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
	m_AutoMap["nop"] = new frc2::SequentialCommandGroup(frc2::PrintCommand("nop"));
	// m_AutoMap["fancy"] = new frc2::SequentialCommandGroup(frc2::WaitCommand(units::second_t(m_Delay)), frc2::PrintCommand("Move"), frc2::PrintCommand("Aligned"), frc2::PrintCommand("Shoot"));
	// m_AutoMap["safe"] = new frc2::SequentialCommandGroup(frc2::WaitCommand(units::second_t(m_Delay)), EncoderDriveV(0.0, -24.0, 0));

	//m_AutoMap["nop"] = new frc2::SequentialCommandGroup(frc2::PrintCommand("Init"), frc2::WaitCommand(units::second_t(m_Delay)), frc2::PrintCommand("Did Nothing"));

	frc2::SequentialCommandGroup* IntakeAuto = new frc2::SequentialCommandGroup();
	IntakeAuto->AddCommands(frc2::InstantCommand([&] { Robot::Get().SetNavXOffset(180.0); }, {}));
	IntakeAuto->AddCommands(frc2::InstantCommand([&] {Robot::Get().GetIntake().GetPositioner().Set(ControlMode::PercentOutput, -1); }, {}));
	IntakeAuto->AddCommands(frc2::InstantCommand([&] {Robot::Get().GetIntake().GetSpinner().Set(ControlMode::PercentOutput, 1); }, {}));
	IntakeAuto->AddCommands(EncoderDriveM(0.0, 148.63, 0.5));//Move to trench
	IntakeAuto->AddCommands(frc2::InstantCommand([&] {Robot::Get().GetIntake().GetSpinner().Set(ControlMode::PercentOutput, 0);}, {}));//Stop intake
	IntakeAuto->AddCommands(TurnToPosPID(180.0));//Turn around (facing target)
	IntakeAuto->AddCommands(EncoderDriveM(0.0, 72.0, 0.5)); //Move to end of trench
	IntakeAuto->AddCommands(TurnToPosSlow());//Aim
	IntakeAuto->AddCommands(Robot::Get().GetShooter().Shoot(10.0));


	// IntakeAuto->AddCommands(frc2::InstantCommand( [&] { Robot::Get().GetShooter().GetFlywheel().Set(ControlMode::Velocity, 3145 / 600 * 2048); }, {}));
	// IntakeAuto->AddCommands(frc2::InstantCommand([&] {
	// 	 Robot::Get().GetShooter().GetLowConveyor().Set(ControlMode::PercentOutput, 0.5);
	// 	 Robot::Get().GetShooter().GetHighConveyor().Set(ControlMode::PercentOutput, 1);
	// }, {}));
	// IntakeAuto->AddCommands(frc2::WaitCommand(units::second_t(1)));
	// IntakeAuto->AddCommands(frc2::RunCommand([&] { Robot::Get().GetShooter().GetFeeder().Set(ControlMode::PercentOutput, 1); }, {}));//Shoot all 5 balls
	
	m_AutoMap["5ballauto"] = IntakeAuto;

	frc2::SequentialCommandGroup* shootAndBackwards = new frc2::SequentialCommandGroup();
	shootAndBackwards->AddCommands(TurnToPosSlow());
	shootAndBackwards->AddCommands(Robot::Get().GetShooter().Shoot(4.0));
	shootAndBackwards->AddCommands(EncoderDriveV(0.0, -5*12.0, 0));
	m_AutoMap["shoot&back"] = shootAndBackwards;


	frc2::SequentialCommandGroup* shootAndForwards = new frc2::SequentialCommandGroup();
	shootAndForwards->AddCommands(TurnToPosSlow());
	shootAndForwards->AddCommands(Robot::Get().GetShooter().Shoot(4.0));
	shootAndForwards->AddCommands(EncoderDriveV(0.0, 5*12.0, 0));
	m_AutoMap["shoot&forwards"] = shootAndForwards;

	frc2::SequentialCommandGroup* onlyShoot = new frc2::SequentialCommandGroup();
	onlyShoot->AddCommands(frc2::PrintCommand("Init"));
	onlyShoot->AddCommands(TurnToPosSlow());
	onlyShoot->AddCommands(Robot::Get().GetShooter().Shoot(4.0));
	m_AutoMap["onlyShoot"] = onlyShoot;

	frc2::SequentialCommandGroup* onlyShootNoAlign = new frc2::SequentialCommandGroup();
	onlyShootNoAlign->AddCommands(frc2::PrintCommand("Init"));
	onlyShootNoAlign->AddCommands(Robot::Get().GetShooter().Shoot(4.0));
	m_AutoMap["onlyShootNoAlign"] = onlyShootNoAlign;


	frc2::SequentialCommandGroup* onlyBackwards = new frc2::SequentialCommandGroup();
	onlyBackwards->AddCommands(frc2::PrintCommand("Init"));
	m_AutoMap["onlyBackwards"] = onlyBackwards;
	
	
	//m_AutoMap["shoot-backward"] = new frc2::SequentialCommandGroup(frc2::PrintCommand("Init"), frc2::WaitCommand(units::second_t(m_Delay)), TurnToPosSlow(), Robot::Get().GetShooter().Shoot(), EncoderDriveV(0.0, -24.0, 0));
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
	// return m_AutoMap[m_InUse];
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