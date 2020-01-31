#pragma once

#include <string>
#include <map>

#include <networktables/NetworkTable.h>
#include <networktables/NetworkTableEntry.h>
#include <networktables/NetworkTableInstance.h>
#include <frc2/Timer.h>

#include "CobConstants.h"

namespace ohs2020 {

class Cob {
public:
	//Sets up the internal state needed to handle communications with the COB
	//Should be called exactly once on application startup
	static void Init();
	
	static void SendMessage(CobMessageOut key, std::string value){
		if (EnsureExists(key)) s_OutMap[key].SetString(value);
	}


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
	static bool EnsureExists(CobMessageOut key);
	static void RegisterKey(CobKey key, std::string name, bool persistent = false);
	static void RegisterMessageOut(CobMessageOut key, std::string name);

private:
	static nt::NetworkTableInstance s_Table;
	static std::map<CobKey, nt::NetworkTableEntry> s_Map;
	static std::map<CobMessageOut, nt::NetworkTableEntry> s_OutMap;
};

}//frc2019

