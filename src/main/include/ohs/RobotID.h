#pragma once

#include <frc/DriverStation.h>

namespace ohs623 {

enum Motor {
	FRONT_LEFT, FRONT_RIGHT, BACK_LEFT, BACK_RIGHT,
	MOTOR_COUNT
};

enum BotType {
	MARK, SABER,
	BOT_TYPE_COUNT
};

class RobotID {    

public:
	RobotID() = delete;

	static int GetID(Motor);

	inline static BotType GetCurrentBot() { return s_CurrentBot; }

private:
	static BotType s_CurrentBot;

};

} // namepsace