#pragma once

#include <frc2/command/InstantCommand.h>
#include <wpi/SmallSet.h>
namespace ohs2020{

class ToggleBoolean : public frc2::InstantCommand {

public: 
	ToggleBoolean(bool*, std::function<void(bool newValue)> onFlip);

	void Initialize();

private:

	bool* ptr;
    std::function<void(bool newValue)> onFlip;
};
}//namespace