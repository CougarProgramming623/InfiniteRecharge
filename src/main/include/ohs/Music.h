#pragma once

#include <ctre/phoenix/music/Orchestra.h>
#include <frc2/command/button/Button.h>
#include <frc2/command/InstantCommand.h>

namespace ohs623 {

class Music {
public:
	Music();


	static void Init();

	static void Start();
	static void Stop();

	static void Selector();

	static ctre::phoenix::music::Orchestra GetOrchestra() { return m_Orchestra; } 

private:
	static ctre::phoenix::music::Orchestra m_Orchestra;

	static frc2::Button musicPlayer;
	static frc2::Button musicStopper;

	static int songCount;

	
	
};

}
