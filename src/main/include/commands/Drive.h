#pragma once

#include <frc2/command/CommandBase.h>

namespace ohs2020 {

class Drive : public frc2::CommandBase {

public: 
	Drive();
	virtual void Execute() override;

};

}//namespace
