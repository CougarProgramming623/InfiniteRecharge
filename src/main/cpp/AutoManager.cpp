#include "AutoManager.h"

namespace ohs2020{

AutoManager::AutoManager(){}
AutoManager::~AutoManager(){
    for (std::pair<std::string, frc2::Command*> element : m_AutoMap) {
        delete element.second;
    }
}
void AutoManager::AutoInit(){
    m_AutoMap["default"] = new frc2::SequentialCommandGroup(frc2::PrintCommand("Move"), frc2::PrintCommand("Shoot"));
    m_AutoMap["fancy"] = new frc2::SequentialCommandGroup(frc2::PrintCommand("Move"), frc2::PrintCommand("Aligned"), frc2::PrintCommand("Shoot"));
    m_AutoMap["safe"] = new frc2::PrintCommand("Moved Forward");
}

void AutoManager::setInUse(std::string setAuto) {
    if (setAuto != "--DELETED--") {
        m_InUse = setAuto;
    }
 }

frc2::Command* AutoManager::getAuto() {
    return m_AutoMap[m_InUse];
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