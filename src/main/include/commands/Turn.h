#pragma once

#include "Robot.h"

#include <frc/PIDOutput.h>
#include <frc2/command/PIDCommand.h>

namespace ohs623 {

class Turn : public frc2::PIDCommand, public frc::PIDOutput {

public:

	Turn(double);//constructor

	void Initialize() override;

	void Execute() override;

	bool IsFinished() override;

	void End(bool) override;

	virtual void PIDWrite(double) override;

private:

	frc2::PIDController* m_TurnController;

	double m_Angle, m_RotToAngleRate, m_KP, m_KI, m_KD;	

};//end class

}//end namespace