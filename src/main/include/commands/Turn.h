#pragma once

#include "Robot.h"

//#include <frc/PIDOutput.h>
#include <frc2/command/PIDCommand.h>
#include <wpi/ArrayRef.h>

#include <frc2/command/CommandBase.h>
#include <frc2/command/CommandHelper.h>

namespace ohs623 {

class Turn : public frc2::CommandHelper<frc2::CommandBase, Turn> {

public:

	Turn(double);//constructor

	void Initialize() override;

	void Execute() override;

	bool IsFinished() override;

	void End(bool) override;

	void Calculate(double);

	void Calculate(double, double);

private:

	frc2::PIDController* m_TurnController;

	std::function<double()> measurement = []()->double{return (double)(ohs2020::Robot::Get().GetNavX()->GetYaw());};
	std::function<void(double)> output = [this](double measure){this->Calculate(measure);};

	double m_Angle, m_RotToAngleRate, m_KP, m_KI, m_KD;	

};//end class

}//end namespace