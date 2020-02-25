
#include "ohs/Music.h"
#include "ohs/Log.h"

#include "Robot.h"


namespace ohs2020 {

ctre::phoenix::music::Orchestra Music::m_Orchestra;

std::string DuelOfTheFates = "DuelOfTheFates.chrp";

void Music::Init() {
	TalonFX* rightBack = dynamic_cast<TalonFX*>(ohs2020::Robot::Get().GetDriveTrain().GetRBack());
	TalonFX* leftBack = dynamic_cast<TalonFX*>(ohs2020::Robot::Get().GetDriveTrain().GetLBack());
	TalonFX* rightFront = dynamic_cast<TalonFX*>(ohs2020::Robot::Get().GetDriveTrain().GetRFront());
	TalonFX* leftFront = dynamic_cast<TalonFX*>(ohs2020::Robot::Get().GetDriveTrain().GetLFront());
	if (rightBack != nullptr) m_Orchestra.AddInstrument(*rightBack);
	if (leftBack != nullptr) m_Orchestra.AddInstrument(*leftBack);
	if (rightFront != nullptr) m_Orchestra.AddInstrument(*rightFront);
	if (leftFront != nullptr) m_Orchestra.AddInstrument(*leftFront);
	if (!rightBack && !leftBack && !rightFront && !leftFront) {
		OHS_WARN([](auto& f) {
			f << "No Talon FX's avilable on robot to play music with!";
		});
	} else {
		OHS_DEBUG([](auto& f) {
			f << "Found Talon FX's to play music with!";
		});
	}

	Start();

}

void Music::Start() {

	m_Orchestra.LoadMusic(DuelOfTheFates);

	OHS_DEBUG([](auto& f) {
		f << "Music::Start()";
	});

	m_Orchestra.Play();
}

void Music::Stop() {

}

}