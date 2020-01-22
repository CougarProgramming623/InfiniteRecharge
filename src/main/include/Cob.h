#pragma once

#include <string>
#include <map>

#include <networktables/NetworkTable.h>
#include <networktables/NetworkTableEntry.h>
#include <networktables/NetworkTableInstance.h>

#include "CobConstants.h"


namespace ohs2020 {

class Cob {
public:
	//Sets up the internal state needed to handle communications with the COB
	//Should be called exactly once on application startup
	static void Init();

	template<typename T>
	static void PushValue(CobKey key, T value) {
		if (EnsureExists(key)) s_Map[key].SetDouble(value);
	}
	
	template<typename T>
	static T GetValue(CobKey key) {
		//Default values dont matter here because if the key doesnt exist in the map c++ will throw an exception
		return s_Map[key].GetDouble(-1.0);
	}


private:
	static bool EnsureExists(CobKey key);
	static void RegisterKey(CobKey key, std::string name, bool persistent = false);

private: 
	static std::shared_ptr<NetworkTable> s_Table;
	static std::map<CobKey, nt::NetworkTableEntry> s_Map;
};

}//frc2019

