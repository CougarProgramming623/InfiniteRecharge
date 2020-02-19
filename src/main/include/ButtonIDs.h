#pragma once

#define JoystickID 				Robot::Get().GetOI().GetDriverJoystick().GetRawButton
#define ButtonID 				Robot::Get().GetOI().GetButtonBoard().GetRawButton

//climb	
#define CLIMB_UP_ID				(19)	//Green Climb Button
#define CLIMB_DOWN_ID			(20)	//Red Climb Button
#define CLIMB_LEFT_ID			(21)
#define CLIMB_RIGHT_ID			(22)
#define DEPLOYER_ID				(17) //Drive Override Switch
#define ENDGAME_OVERRIDE_ID		(18) //Endgame Override Switch

//shooter
#define LAUNCHER_ID				(6) //Arm Override Button
#define FLYWHEEL_TOGGLE_ID		(1) //Vacuum Switch

//intake
#define INTAKE_DOWN_ID			(10) //Bot Heading Dial
#define INTAKE_UP_ID			(9)//Bot Heading Dial
#define INTAKE_MANUAL_ID		(11)//Bot Heading Dial
#define INTAKE_TOGGLE_ID		(11) //Fork Override Button

//OI
#define FOD_TOGGLE_ID			(1) //Joystick Button
#define TURN_ID					(14)//Hatch/Cargo Switch