#include "Cob.h"

#include <frc/DriverStation.h>
#include <sstream>

namespace ohs2020 {

nt::NetworkTableInstance Cob::s_Table;
std::map<CobKey, nt::NetworkTableEntry> Cob::s_Map;
std::map<CobMessageOut, nt::NetworkTableEntry> Cob::s_OutMap;


void Cob::Init() {
	s_Table = nt::NetworkTableInstance::GetDefault();
	//Clean up the old values in the table every time we init
	//Consider changing once we test out persistent values
	s_Table.DeleteAllEntries();

    RegisterKey(CobKey::ROTATION, "/cob/location/rotation");
    RegisterKey(CobKey::TIME_LEFT, "/cob/fms/time-left");
    RegisterKey(CobKey::IS_RED, "/FMSInfo/IsRedAlliance");
    RegisterKey(CobKey::MODE, "/cob/mode");
    RegisterKey(CobKey::GYRO_RESET, "/cob/actions/gyroReset");
	RegisterMessageOut(CobMessageOut::PING, "ping");
}

void Cob::RegisterKey(CobKey key, std::string name, bool persistent) {
	nt::NetworkTableEntry entry = s_Table.GetEntry(name);
	s_Map[key] = entry;
	if (persistent)
		entry.SetPersistent();
}

void Cob::RegisterMessageOut(CobMessageOut key, std::string name) {
	nt::NetworkTableEntry entry = s_Table.GetEntry("/cob/messages/cob/" + name);
	s_OutMap[key] = entry;
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

bool Cob::EnsureExists(CobMessageOut key) {
	if (s_OutMap.find(key) == s_OutMap.end()) {
		//The value doesnt exist in the map
		std::stringstream ss;
		ss << "Failed to find registered key for CobMessageOut value #" << static_cast<int>(key);
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
void Cob::PushValue<int>(CobKey key, int value) {
	if (EnsureExists(key)) s_Map[key].SetDouble(value);
}

template<>
void Cob::PushValue<std::string>(CobKey key, std::string value) {
	if (EnsureExists(key)) s_Map[key].SetString(value);
}

// template<>
// void Cob::SendMessage<bool>(CobMessageOut key, bool value) {
// 	if (EnsureExists(key)) s_OutMap[key].SetBoolean(value);
// }

// template<>
// void Cob::SendMessage<int>(CobMessageOut key, int value) {
// 	if (EnsureExists(key)) s_OutMap[key].SetDouble(value);
// }

// template<>
// void Cob::SendMessage<std::string>(CobMessageOut key, std::string value) {
// 	if (EnsureExists(key)) s_OutMap[key].SetString(value);
// }

// template<>
// void Cob::SendMessage<const char*>(CobMessageOut key, const char* value) {
// 	if (EnsureExists(key)) s_OutMap[key].SetString(value);
// }

template<>
bool Cob::GetValue(CobKey key) {
	return s_Map[key].GetBoolean(false);
}

template<>
std::string Cob::GetValue(CobKey key) {
	return s_Map[key].GetString("");
}

}//namesapce
