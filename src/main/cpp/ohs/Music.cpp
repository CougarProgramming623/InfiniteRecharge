#include "ohs/Log.h"

#include "Robot.h"

#include "commands/PlayMusic.h"


namespace ohs623 {

ctre::phoenix::music::Orchestra Music::m_Orchestra;

frc2::Button Music::musicPlayer( [&] { return ohs2020::Robot::Get().GetOI().GetButtonBoard().GetRawButton(9); });
frc2::Button Music::musicStopper( [&] { return ohs2020::Robot::Get().GetOI().GetButtonBoard().GetRawButton(10); });

bool isPaused = false;

std::string DuelOfTheFates = "DuelOfTheFates.chrp";
std::string StarWars = "StarWarsMainTheme.chrp";
std::string JurassicPark = "JurassicParkMainTheme.chrp";
std::string ImperialSuite = "ImperialSuite.chrp";
std::string CoconutMall = "CoconutMall.chrp";


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

	m_Orchestra.LoadMusic(CoconutMall);
	DebugOutF("Loaded Music");
	
	musicPlayer.WhenPressed(PlayMusic());
	musicStopper.WhenPressed(frc2::InstantCommand([&] { 

	if(!isPaused){
		m_Orchestra.Pause();
		isPaused = true;
	} else {
		m_Orchestra.Stop();
		isPaused = false;
	}

	}, {}));
}

void Music::Stop() {

}



void Music::Selector() {

	double selection = ohs2020::Robot::Get().GetOI().GetButtonBoard().GetRawAxis(1);

	if(selection > (songCount - 2) / songCount ){ //scans 0 zone

	} else if (selection < -(songCount - 2) / songCount && selection > songCount - 2){ //scans pos zone

	} else if ( selection > -(songCount - 2) / songCount && selection < songCount - 2) { // scans neg zone

	}



}

}