#include "Cob.h"

#include <frc/DriverStation.h>
#include <sstream>

namespace ohs2020 {

std::shared_ptr<nt::NetworkTable> Cob::s_Table;
std::map<CobKey, nt::NetworkTableEntry> Cob::s_Map;

void Cob::Init() {
    nt::NetworkTableInstance instance = nt::NetworkTableInstance::GetDefault();
    s_Table = instance.GetTable("cob");
	//Clean up the old values in the table every time we init
	//Consider changing once we test out persistent values
	instance.DeleteAllEntries();

	RegisterKey(CobKey::ROBOT_POSITION_X, "robot/position/x");
	RegisterKey(CobKey::ROBOT_POSITION_Y, "robot/position/y");
	//...

}

void Cob::RegisterKey(CobKey key, std::string name, bool persistent) {
	nt::NetworkTableEntry entry = s_Table->GetEntry(name);
	s_Map[key] = entry;
	if (persistent)
		entry.SetPersistent();
		
}

bool Cob::EnsureExists(CobKey key) {
	if (s_Map.find(key) == s_Map.end()) {
		//The value doesnt exist in the map
		std::stringstream ss;
		ss << "Failed to find registered key for CobKey value #" << static_cast<int>(key);
		frc::DriverStation::ReportError(ss.str());
		return false;
	} else {
		return true;//The value exists
	}
}

//Explicit specialization for non numeric types
template<>
void Cob::PushValue<bool>(CobKey key, bool value) {
	if (EnsureExists(key)) s_Map[key].SetBoolean(value);
}

template<>
void Cob::PushValue<std::string>(CobKey key, std::string value) {
	if (EnsureExists(key)) s_Map[key].SetString(value);
}


template<>
bool Cob::GetValue(CobKey key) {
	return s_Map[key].GetBoolean(false);
}

template<>
std::string Cob::GetValue(CobKey key) {
	return s_Map[key].GetString("");
}

}//namesapce
