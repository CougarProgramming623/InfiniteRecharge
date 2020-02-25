#pragma once

#include <ctre/phoenix/music/Orchestra.h>

namespace ohs2020 {

class Music {
public:

	static void Init();

	static void Start();
	static void Stop();

private:
	static ctre::phoenix::music::Orchestra m_Orchestra;
	
};

}
