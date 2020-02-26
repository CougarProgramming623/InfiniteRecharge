#include "ohs/Log.h"

#include "Robot.h"

#include "commands/PlayMusic.h"

#include <string>


namespace ohs623 {

ctre::phoenix::music::Orchestra Music::m_Orchestra;

frc2::Button Music::musicPlayer( [&] { return ohs2020::Robot::Get().GetOI().GetButtonBoard().GetRawButton(9); });
frc2::Button Music::musicStopper( [&] { return ohs2020::Robot::Get().GetOI().GetButtonBoard().GetRawButton(10); });

bool isPaused = false;

char songs [15][27] = {

	"DuelOfTheFates.chrp", //Star Wars
	"StarWarsMainTheme.chrp",
	"ImperialSuite.chrp",
	"ImperialMarch.chrp",

	"JurassicParkMainTheme.chrp", //Jurassic Park

	"RaidersOftheLostArk.chrp", //Indiana Jones

	"PiratesOfTheCaribbean.chrp", //Pirates of the Caribbean

	"ImmigrantSong.chrp", //Marvel

	"Megalovania.chrp", //Nintendo
	"BunnyTime.chrp", 

	"RussianAnthem.chrp", //Anthems
	"SovietAnthem.chrp",
	"AmericanAnthem.chrp",

	"CoconutMall.chrp", //Misc
	"HomeDepotTheme.chrp"
	
};

static int songCount = sizeof(songs);
std::string songToPlay = std::string(songs[0]);

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

	musicPlayer.WhenPressed(frc2::InstantCommand( [&] {
		isPaused = false;
		frc2::CommandScheduler::GetInstance().Schedule(new PlayMusic());
	}, {} ));

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

void Music::Selector() {

	double selection = (int)((ohs2020::Robot::Get().GetOI().GetButtonBoard().GetRawAxis(1) + 1)/ 2 * songCount);

	if(selection == songCount) selection--; //removes extra option at end of slide (last digit)

	for(int i = 0; i < songCount; i++){
		if(selection == i){
			DebugOutF(std::string(songs[i]));
			songToPlay = std::string(songs[i]);
			m_Orchestra.LoadMusic(songToPlay);
			DebugOutF(songToPlay);
		}
	}
}

}//namespace