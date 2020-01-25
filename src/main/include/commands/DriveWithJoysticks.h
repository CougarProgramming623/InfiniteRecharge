#pragma once

#include <frc2/command/CommandBase.h>
#include <frc2/command/CommandHelper.h>

namespace ohs2020 {

class Drive : public frc2::CommandHelper<frc2::CommandBase, Drive> {

public: 
	Drive();
	virtual void Execute() override;
	virtual void Initialize() override;

	virtual std::unique_ptr<Command> TransferOwnership() && override;

// private:
// 	DriveTrain* m_subsystem;

};

}//namespace
