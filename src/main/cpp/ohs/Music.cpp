
#include "ohs/Music.h"
#include "ohs/Log.h"

#include "Robot.h"

namespace ohs623 {

ctre::phoenix::music::Orchestra Music::m_Orchestra;

void Music::Init() {
	TalonFX* rb = dynamic_cast<TalonFX*>(ohs2020::Robot::Get().GetDriveTrain().GetRBack());
	TalonFX* lb = dynamic_cast<TalonFX*>(ohs2020::Robot::Get().GetDriveTrain().GetLBack());
	TalonFX* rf = dynamic_cast<TalonFX*>(ohs2020::Robot::Get().GetDriveTrain().GetRFront());
	TalonFX* lf = dynamic_cast<TalonFX*>(ohs2020::Robot::Get().GetDriveTrain().GetLFront());
	if (rb != nullptr) m_Orchestra.AddInstrument(*rb);
	if (lb != nullptr) m_Orchestra.AddInstrument(*lb);
	if (rf != nullptr) m_Orchestra.AddInstrument(*rf);
	if (lf != nullptr) m_Orchestra.AddInstrument(*lf);
	if (!rb && !lb && !rf && !lf) {
		OHS_WARN([](auto& f) {
			f << "No Talon FX's avilable on robot to play music with!";
		});
	}



}

void Music::Start() {

}

void Music::Stop() {

}


}
