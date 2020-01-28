#include "commands/ToggleBoolean.h"

namespace ohs2020{

ToggleBoolean::ToggleBoolean(bool* toggle, std::function<void(bool newValue)> onFlip) : ptr(toggle), onFlip(onFlip) {


}

void ToggleBoolean::Initialize() {
	*ptr = !ptr;

	onFlip(*ptr);

}

}//namespace