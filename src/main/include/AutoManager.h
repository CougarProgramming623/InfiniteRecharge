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

#include "CobConstants.h"

namespace ohs2020{

class AutoManager{

public:
    AutoManager();

    void AutoInit();


private:
    std::map<std::string, frc2::Command> m_AutoMap;
    std::string m_InUse = "Default";
};
}