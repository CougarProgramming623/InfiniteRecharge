#pragma once

#include "Cob.h"
#include "Robot.h"

#include <frc2/command/PIDCommand.h>
#include <frc2/command/CommandBase.h>
#include <frc2/command/CommandHelper.h>

namespace frc2020{

class TurnToPosPID : public frc2::CommandHelper<frc2::PIDCommand, TurnToPosPID> {

public: 
    TurnToPosPID();
    TurnToPosPID(double angle);
    void Initialize() override;
	void Execute() override;
	bool IsFinished() override;
	void End(bool interrupted) override;

    static frc2::PIDController CreateTurnController();

private:
    frc2::PIDController* m_TurnController;
};

} // end namespace