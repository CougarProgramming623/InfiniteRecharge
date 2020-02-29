#pragma once
//#prarma once amiright?

#include <networktables/NetworkTable.h>
#include <networktables/NetworkTableEntry.h>
#include <networktables/NetworkTableInstance.h>
#include <map>
#include <frc2/command/InstantCommand.h>
#include <frc2/command/RunCommand.h>
#include <frc2/command/Command.h>	
#include <frc2/command/SequentialCommandGroup.h>
#include <frc2/command/PrintCommand.h>
#include <frc2/command/CommandScheduler.h>
#include <frc2/command/WaitCommand.h>
#include "ohs/Log.h"

#include "CobConstants.h"
#include "Cob.h"

namespace ohs2020 {

class AutoManager {

public:
	AutoManager();
	~AutoManager();

	void AutoInit();
	void RunAuto();

	inline std::string GetInUse() {return m_InUse; }
	inline double GetDelay() { return m_Delay; }
	frc2::Command* GetAuto();
	void SetInUse(std::string setAuto);
	inline void SetDelay(double setDelay) {m_Delay = setDelay;}

private:
	std::map<std::string, frc2::Command*> m_AutoMap;
	std::string m_InUse = "shoot&back";
	double m_Delay = 0;
};
}