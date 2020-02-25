#pragma once

#include "Cob.h"
#include "Robot.h"

#include <frc2/command/PIDCommand.h>

namespace ohs2020 {

class TurnToPosPID : public frc2::PIDCommand {

public: 
    ~TurnToPosPID(){}

    TurnToPosPID();
    TurnToPosPID(double angle);

    void Initialize() override;
    void Execute() override;
	bool IsFinished() override;
	void End(bool interrupted) override;

    frc2::PIDController CreateTurnController();
    double GetVisionAngle();

private:
    frc2::PIDController* m_TurnController;
    double m_Angle = 0.0;
    double m_Offset = 0.0;
    bool m_IsVision;
    std::array<double, 10> m_OffsetValues;
    int m_Index = 0;
};

} // end namespace
