#pragma once

#include <frc2/command/Command.h>
#include <wpi/SmallSet.h>
#include <functional>

namespace ohs2020{

class ToggleBoolean : public frc2::Command {

public: 
	ToggleBoolean(bool*, std::function<void(bool newValue)> onFlip);

	void Initialize() override;


	virtual void Execute() override;
	virtual bool IsFinished() override;
	virtual void End(bool) override;

	virtual wpi::SmallSet<frc2::Subsystem*, 4> GetRequirements() const override;
	virtual std::unique_ptr<frc2::Command> TransferOwnership() && override;

private:

	bool* ptr;
    std::function<void(bool newValue)> onFlip;
};
}//namespace