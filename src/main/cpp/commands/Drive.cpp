#include "commands/Drive.h"
#include "Robot.h"


namespace ohs2020 {

    Drive::Drive(){}

    void Drive::Execute(){

        using namespace ohs2020;

        double y = Robot::Get().GetOI().GetDriverJoystick().GetY();
        double x = Robot::Get().GetOI().GetDriverJoystick().GetX();
        double rot = Robot::Get().GetOI().GetDriverJoystick().GetZ();
        //double gyro = Robot::navx->GetYaw();

        y = abs(y) <= 0.075f ? 0 : y;
        x = abs(x) <= 0.075f ? 0 : x;
        rot = abs(rot) <= 0.05f ? 0 : rot;

        if(Robot::Get().GetOI().IsFOD()) {
            Robot::Get().GetDriveTrain().CartesianDrive(y, x, rot/2, 0.0);
        } else {
            Robot::Get().GetDriveTrain().CartesianDrive(y, x, rot/2, 0.0);
        }
    }




}