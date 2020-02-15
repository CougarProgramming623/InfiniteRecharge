#pragma once

#define JoystickID 				Robot::Get().GetOI().GetDriverJoystick().GetRawButton
#define ButtonID 				Robot::Get().GetOI().GetButtonBoard().GetRawButton

//climb	
#define CLIMB_UP_ID				(2)	//Green Climb Button
#define CLIMB_DOWN_ID			(3)	//Red Climb Button
#define CLIMB_LEFT_ID			(-1)
#define CLIMB_RIGHT_ID			(-1)
#define DEPLOYER_ID				(5) //Drive Override Switch
#define ENDGAME_OVERRIDE_ID		(4) //Endgame Override Switch

//shooter
#define LAUNCHER_ID				(6) //Arm Override Button
#define FLYWHEEL_TOGGLE_ID		(1) //Vacuum Switch

//intake
#define INTAKE_DOWN_ID			(9) //Bot Heading Dial
#define INTAKE_UP_ID			(10)//Bot Heading Dial
#define INTAKE_MANUAL_ID		(11)//Bot Heading Dial
#define INTAKE_TOGGLE_ID		(7) //Fork Override Button

//OI
#define FOD_TOGGLE_ID			(1) //Joystick Button
#define TURN_ID					(17)//Hatch/Cargo Switch