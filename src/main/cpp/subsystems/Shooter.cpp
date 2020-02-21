#include "subsystems/Shooter.h"
#include "Robot.h"
#include "ohs/RobotID.h"
#include "ohs/Log.h"

#include <frc/smartdashboard/SmartDashboard.h>

#include "frc2/command/FunctionalCommand.h"

using namespace ohs623;

namespace ohs2020{

const double DefaultShooterPower = 1;

Shooter::Shooter() : 

Flywheel(RobotID::GetID(FLYWHEEL)),
feeder(RobotID::GetID(FEEDER)),
launcher( [&] { return Robot::Get().GetOI().GetButtonBoard().GetRawButton(6); }), // Arm Override
flyWheelToggle([&] { return Robot::Get().GetOI().GetButtonBoard().GetRawButton(1); }), //Vacuum Toggle Switch
FlyWheelEncoder(RobotID::GetID(FLYWHEEL)),
timer()
{}

void Shooter::Init() {

	SetupShooterButtons();
	
}

inline void Shooter::SetupShooterButtons() {

	flyWheelToggle.WhileHeld(frc2::FunctionalCommand([this]{}, [this] { //on execute

		isFlywheelOn = true;
<<<<<<< HEAD
		flywheelWU = Flywheel.GetSelectedSensorVelocity() / 4;
=======
		flywheelWU = (int)((double)Flywheel.GetSelectedSensorVelocity() / 2048 * 600);
		DebugOutF(std::to_string(flywheelWU));
>>>>>>> shooter
		frc::SmartDashboard::PutNumber("Flywheel Speed", flywheelWU);

		Flywheel.Set(ControlMode::PercentOutput, Robot::Get().GetOI().GetButtonBoard().GetRawAxis(0));

	}, [this] (bool f){//on end

		isFlywheelOn = false;

		Flywheel.Set(ControlMode::PercentOutput, 0);

	}, [this] { return false; }, {}));

	flyWheelToggle.WhenReleased(frc2::RunCommand([&] {

		flywheelWU = (int)((double)Flywheel.GetSelectedSensorVelocity() / 2048 * 600);
		DebugOutF(std::to_string(flywheelWU));

<<<<<<< HEAD
	}, [this] { // is finished
		
		OHS_DEBUG([&](auto& f){ f << "shooter is finished? " << (timer.Get() > units::second_t(1)); })
		return timer.Get() > units::second_t(2);


	}, {});
	for(int i = 0; i < 100; i++){
		vector.push_back(std::unique_ptr<frc2::Command>(shootBall));
		vector.push_back(std::make_unique<frc2::WaitCommand>(units::second_t(1)));
	}
	launcher.WhenHeld(frc2::SequentialCommandGroup(std::move(vector)));

}
=======
	}, {}));
} 
>>>>>>> shooter
}//namespace