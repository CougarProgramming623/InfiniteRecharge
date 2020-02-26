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
std::map<CobKey, OHSNetworkTableEntry> Cob::s_Map;
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

void Cob::Init() {
	s_Table = nt::NetworkTableInstance::GetDefault();

	//Clean up the old values in the table every time we init
	//Consider changing once we test out persistent values
	s_Table.DeleteAllEntries();


    RegisterKey(CobKey::ROTATION, "/cob/location/rotation");
    RegisterKey(CobKey::TIME_LEFT, "/cob/fms/time-left");
    RegisterKey(CobKey::IS_RED, "/FMSInfo/IsRedAlliance");
    RegisterKey(CobKey::MODE, "/cob/mode");

	RegisterMessageOut(CobMessageOut::PING, "ping");
	RegisterMessageIn(CobMessageIn::GNIP, "gnip", Handshake);
	RegisterMessageIn(CobMessageIn::GYRO_RESET, "gyroReset", GyroResetConfirm);
	RegisterMessageOut(CobMessageOut::GYRO_RESET_CONFIRM, "gyroReset-ack");
	RegisterMessageIn(CobMessageIn::RECEIVE_AUTO,"setAuto", ReceiveAuto);

	RegisterKey(CobKey::FLYWHEEL_WU, "/cob/flywheel/wu");
	RegisterKey(CobKey::FLYWHEEL_STATUS, "/cob/flywheel/image");
	RegisterKey(CobKey::IN_USE_AUTO, "/cob/auto/in-use");

	RegisterKey(CobKey::LIMELIGHT_TOGGLE, "/limelight/ledMode");

	LoadPersistent();
}

void Cob::RegisterKey(CobKey key, std::string name, bool persistent) {
	nt::NetworkTableEntry entry = s_Table.GetEntry(name);
	if (s_Map.find(key) == s_Map.end()) {
		s_Map[key] = { entry, persistent };

	}
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


template<>
void Cob::PushValue<bool>(CobKey key, bool value) {
	if (EnsureExists(key)) s_Map[key].Entry.SetBoolean(value);
}

template<>
void Cob::PushValue<double>(CobKey key, double value) {
	if (EnsureExists(key)) s_Map[key].Entry.SetDouble(value);
}

template<>
void Cob::PushValue<int>(CobKey key, int value) {
	if (EnsureExists(key)) s_Map[key].Entry.SetDouble(value);
}

template<>
void Cob::PushValue<float>(CobKey key, float value) {
	if (EnsureExists(key)) s_Map[key].Entry.SetDouble(value);
}

template<>
void Cob::PushValue<std::string>(CobKey key, std::string value) {
	if (EnsureExists(key)) s_Map[key].Entry.SetString(value);
}

template<>
void Cob::PushValue<const char*>(CobKey key, const char* value) {
	if (EnsureExists(key)) s_Map[key].Entry.SetString(value);
}

template<>
void Cob::PushValue<std::string&>(CobKey key, std::string& value) {
	if (EnsureExists(key)) s_Map[key].Entry.SetString(value);
}


template<>
bool Cob::GetValue(CobKey key) {
	return s_Map[key].Entry.GetBoolean(false);
}

template<>
std::string Cob::GetValue(CobKey key) {
	return s_Map[key].Entry.GetString("");
}


struct SerializedEntry {
	char Path[32];
	nt::NetworkTableType Type;
	CobKey Key;
};

//Returns the number of characters used by the c-string str (not including the null termination character)
//Or maxSize if there is no null byte with the first [0, maxSize) bytes
std::size_t StrlenOrSize(const char* str, std::size_t maxSize) {
	for (std::size_t i = 0; i < maxSize; i++) {
		if (str[i] == 0x00) return i;
	}
	return maxSize;
	
}

//Decodes the value of a persistent entry from the stream and pushes it to the cob
bool Cob::DecodePersistentData(nt::NetworkTableType type, CobKey cobKey, FILE* file) {
	if (s_Map.find(cobKey) == s_Map.end()) {
		OHS_ERROR([&](auto& f) { f << "Failed to find NT entry with the cobkey " << static_cast<int>(cobKey); });
		return false;
	} else {
		nt::NetworkTableEntry& ntEntry = s_Map[cobKey].Entry;
		switch (type) {
			case nt::NetworkTableType::kUnassigned:

				return true;
			case nt::NetworkTableType::kBoolean:
			{
				unsigned char value;
				fread(&value, sizeof(value), 1, file);

				PushValue(cobKey, (value == 0) ? false : true);
				OHS_INFO([&](auto& f) { f << "Decoded persistent NT entry boolean " << value << " for value " << ntEntry.GetName(); });

				return true;
			}
			case nt::NetworkTableType::kDouble:
			{
				double value;
				fread(&value, sizeof(value), 1, file);
				OHS_INFO([&](auto& f) { f << "Decoded persistent NT entry double " << value << " for value " << ntEntry.GetName(); });

				PushValue(cobKey, value);
				return true;
			}
			case nt::NetworkTableType::kString:
			{
				uint32_t length;
				fread(&length, sizeof(length), 1, file);
				if (length > 128) {
					OHS_WARN([&](auto& f) { f << "Large NT persistent entry size: " << length << " for value " << ntEntry.GetName(); });
				}
				if (length > 1024) {
					OHS_ERROR([&](auto& f) { f << "Persistent NT entry size too large! " << length << " for value " << ntEntry.GetName() << ". File may be corrupt"; });
					return false;
				}
				std::string value;
				value.resize(length);
				fread(value.data(), length, 1, file);
				OHS_INFO([&](auto& f) { f << "Decoded persistent NT entry string " << value << " for value " << ntEntry.GetName(); });
				PushValue(cobKey, value);
				return true;
			}
			case nt::NetworkTableType::kRaw:
				goto Unimplemented;

			case nt::NetworkTableType::kBooleanArray:
				goto Unimplemented;

			case nt::NetworkTableType::kDoubleArray:
				goto Unimplemented;

			case nt::NetworkTableType::kStringArray:
				goto Unimplemented;

			case nt::NetworkTableType::kRpc:
				goto Unimplemented;

			default:
				OHS_ERROR([&](auto& f) { f << "Unknown NT type " << static_cast<int>(type) << " for value " << ntEntry.GetName(); });
				return false;

		}

		Unimplemented:
			OHS_ERROR([&](auto& f) { f << "Unable to decode the NT type: " << static_cast<int>(type) << " for value " << ntEntry.GetName(); });
			return false;
	}
}

void Cob::EncodePersistentData(CobKey cobKey, FILE* file) {
	
}


void Cob::LoadPersistent() {
	DriverStation::ReportError("Load begin...");

	int deleteCount = 0;
	for (auto entry : s_Table.GetEntries(nt::StringRef(nullptr, 0), 0)) {
		entry.Delete();
		deleteCount++;
	}
	DriverStation::ReportError("for \"\" Deleting: " + std::to_string(deleteCount));

	FILE* file = fopen("~/CobPersistentValues.bat", "rb");
	if (file == nullptr) {
		OHS_WARN([&](auto& f) { f << "[Persistent COB] Unable to find values file"; });
	} else {
		OHS_INFO([&](auto& f) { f << "[Persistent COB] Found pre-existing values file"; });
		SerializedEntry entry;
		while (fread(&entry, sizeof(entry), 1, file) == sizeof(entry)) {
			std::size_t pathLength = StrlenOrSize(entry.Path, sizeof(entry.Path));
			std::string path(entry.Path, pathLength);
			for (auto& mapEntry : s_Map) {
				bool namesMatch = mapEntry.second.Entry.GetName() == path;
				bool cobKeysMatch = mapEntry.first == entry.Key;
				bool typesMatch = mapEntry.second.Entry.GetType() == entry.Type;
				int matchCount = namesMatch + cobKeysMatch + typesMatch;
				
				if (matchCount == 3) {
					//Good persistent key, everything is the same
					if (!DecodePersistentData(entry.Type, entry.Key, file)) goto DeserializeError;
				} else if (cobKeysMatch) {
					OHS_WARN([&](auto& f) { f << "[Persistent COB] Serialized Value " << entry.Path << "changed to " << mapEntry.second.Entry.GetName() << ". Deserialization is continuing..."; });		
					if (!DecodePersistentData(entry.Type, entry.Key, file)) goto DeserializeError;
				} else {
					OHS_ERROR([&](auto& f) { f << "[Persistent COB] Persistent NT value has changed since serialization!"; });
					OHS_ERROR([&](auto& f) {
						f << "[Persistent COB] Persistent NT value has changed since serialization!"; });
					goto DeserializeError;
				}

			}
		}
	}
	

	DriverStation::ReportError("Load end");
	return;

	DeserializeError:
	{
		OHS_ERROR([&](auto& f) { f << "[Persistent COB] Serialization failed "; });
		return;
	}


}

void Cob::SavePersistent() {

}


}//namespace