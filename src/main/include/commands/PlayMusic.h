#pragma once

#include <frc2/command/CommandBase.h>
#include <frc2/command/CommandHelper.h>

namespace ohs2020 {

class PlayMusic : public frc2::CommandHelper<frc2::CommandBase, PlayMusic> {

public: 
	PlayMusic();
	virtual void Execute() override;
	virtual void Initialize() override;
	virtual bool IsFinished() override;
	virtual void End(bool interrupted) override;

};

}//namespace
