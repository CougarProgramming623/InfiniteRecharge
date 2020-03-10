#include "Cob.h"
#include "ohs/Log.h"
#include "Robot.h"

#include <frc/DriverStation.h>
#include <sstream>
#include <Util.h>
#include <ntcore.h>
#include <functional>


namespace ohs2020 {

nt::NetworkTableInstance Cob::s_Table;
std::map<CobKey, nt::NetworkTableEntry> Cob::s_Map;
std::map<CobMessageOut, nt::NetworkTableEntry> Cob::s_OutMap;
std::map<std::string, CobCallBack> Cob::s_InMap;


void Handshake(const nt::NetworkTableEntry& entry) {
	Cob::SendMessage(CobMessageOut::PING, "Confirmed");
}

void ReceiveAuto(const nt::NetworkTableEntry& entry) {
		Robot::Get().GetAutoMan().SetInUse(entry.GetValue()->GetString());
}

void GyroResetConfirm(const nt::NetworkTableEntry& entry) {
	//if (entry.GetBoolean(false) == true){
		Robot::Get().GetNavX()->ZeroYaw();
		Cob::SendMessage(CobMessageOut::GYRO_RESET_CONFIRM, "Reset the navx");
	//}
}


void ReverseConveyor(const nt::NetworkTableEntry& entry) {
		Robot::Get().GetShooter().ReverseConveyor();
}

void SetDelayMess(const nt::NetworkTableEntry& entry){
	//Robot::Get().GetAutoMan().SetDelay(entry.GetDouble(-1));
	std::stringstream stream(entry.GetValue()->GetString());
	double x = 0;
	OHS_DEBUG([&] (auto &f) {f << "X before: " << x;});
	stream >> x;
	OHS_DEBUG([&] (auto &f) {f << "X after:" << x;});
	Robot::Get().GetAutoMan().SetDelay(x);
}

void Cob::Init() {
	s_Table = nt::NetworkTableInstance::GetDefault();

	//Clean up the old values in the table every time we init
	//Consider changing once we test out persistent values
	s_Table.DeleteAllEntries();


    RegisterKey(CobKey::ROTATION, "/cob/location/rotation");
    RegisterKey(CobKey::TIME_LEFT, "/cob/fms/time-left");
    RegisterKey(CobKey::IS_RED, "/FMSInfo/IsRedAlliance");
    RegisterKey(CobKey::MODE, "/cob/mode");
	RegisterKey(CobKey::CURRENT_DELAY, "/cob/auto/current-delay");
	RegisterKey(CobKey::FLYWHEEL_SPEED, "/cob/flywheel/set");

	RegisterMessageOut(CobMessageOut::PING, "ping");
	RegisterMessageIn(CobMessageIn::GNIP, "gnip", Handshake);
	RegisterMessageIn(CobMessageIn::GYRO_RESET, "gyroReset", GyroResetConfirm);
	RegisterMessageOut(CobMessageOut::GYRO_RESET_CONFIRM, "gyroReset-ack");
	RegisterMessageIn(CobMessageIn::RECEIVE_AUTO,"setAuto", ReceiveAuto);
	RegisterMessageIn(CobMessageIn::REMOVE_LEMON, "removeLemon", ReverseConveyor);
	RegisterMessageIn(CobMessageIn::RECEIVE_AUTO, "setAuto", ReceiveAuto);
	RegisterMessageIn(CobMessageIn::RECEIVE_DELAY, "delay", SetDelayMess);
	RegisterKey(CobKey::FLYWHEEL_WU, "/cob/flywheel/wu");
	RegisterKey(CobKey::FLYWHEEL_STATUS, "/cob/flywheel/image");

	RegisterKey(CobKey::VISION_X, "/limelight/tx");
	RegisterKey(CobKey::VISION_Y, "/limelight/ty");
	
	// RegisterKey(CobKey::VISION_X, "/vision/x");
	// RegisterKey(CobKey::VISION_Y, "/vision/y");
	RegisterKey(CobKey::IN_USE_AUTO, "/cob/auto/in-use");
	RegisterKey(CobKey::LIMELIGHT_TOGGLE, "/limelight/ledMode");
	RegisterKey(CobKey::COB_CHECK, "/cob/cob-check");
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


void Cob::RegisterMessageIn(CobMessageIn key, std::string name, CobCallBack handler) {
	s_InMap["/cob/messages/roborio/" + name] = handler;
}

void Cob::InMesUpdate() {
	std::vector<nt::NetworkTableEntry> entries = s_Table.GetEntries("/cob/messages/roborio/", 0);
	for (nt::NetworkTableEntry& e : entries){
		std::string name = e.GetName();
		if(e.GetType() != nt::NetworkTableType::kString || e.GetString("--NOT-EXIST--") != "--DELETED--") {
			if (s_InMap.find(name) != s_InMap.end()) {
				CobCallBack& jeff = s_InMap[e.GetName()];
				jeff(e);
				// e.Delete();
				e.SetString("--DELETED--");
			} else {
				OHS_ERROR([&](auto& f) {
					f << "Name not in map: " << name << "\nMap is: ";
					for (std::map<std::string, CobCallBack>::iterator it = s_InMap.begin(); it != s_InMap.end(); it++) {
						f << it->first << " = " << it->second.operator bool();
					}
				});
			}
		}
		
	}
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
void Cob::PushValue<double>(CobKey key, double value) {
	if (EnsureExists(key)) s_Map[key].SetDouble(value);
}

template<>
void Cob::PushValue<int>(CobKey key, int value) {
	if (EnsureExists(key)) s_Map[key].SetDouble(value);
}

template<>
void Cob::PushValue<float>(CobKey key, float value) {
	if (EnsureExists(key)) s_Map[key].SetDouble(value);
}

template<>
void Cob::PushValue<std::string>(CobKey key, std::string value) {
	if (EnsureExists(key)) s_Map[key].SetString(value);
}

template<>
void Cob::PushValue<const char*>(CobKey key, const char* value) {
	if (EnsureExists(key)) s_Map[key].SetString(value);
}

template<>
void Cob::PushValue<std::string&>(CobKey key, std::string& value) {
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

}//namespace
