#include "commands/ToggleBoolean.h"
#include "Util.h"

namespace ohs2020{

ToggleBoolean::ToggleBoolean(bool* toggle, std::function<void(bool newValue)> onFlip) : ptr(toggle), onFlip(onFlip) {
	
}

void ToggleBoolean::Initialize() {
	
	*ptr = !ptr;
	
	DebugOutF("Ran Initialize");

	onFlip(*ptr);

	DebugOutF(std::to_string(*ptr));


}

bool ToggleBoolean::IsFinished(){ 
	return true; 
}


void ToggleBoolean::End(bool interrupted) {}

void ToggleBoolean::Execute(){

}

wpi::SmallSet<frc2::Subsystem*, 4> ToggleBoolean::GetRequirements() const{ 
	wpi::SmallSet<frc2::Subsystem*, 4> set;
	return set;
}
std::unique_ptr<frc2::Command> ToggleBoolean::TransferOwnership() &&{
	return nullptr;
}


}//namespace