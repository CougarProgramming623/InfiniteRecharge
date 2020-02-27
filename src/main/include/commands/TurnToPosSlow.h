#pragma once

#include <frc2/command/CommandBase.h>
#include <frc2/command/CommandHelper.h>

namespace ohs2020 {

class TurnToPosSlow : public frc2::CommandHelper<frc2::CommandBase, TurnToPosSlow> {

    public:
        TurnToPosSlow();

        void Initialize() override;
        void Execute() override;
	    bool IsFinished() override;
	    void End(bool interrupted) override;

        double GetVisionAngle();

    private:
        double m_Angle = 0.0;
        bool m_StartNeg = false;

};

} // namespace