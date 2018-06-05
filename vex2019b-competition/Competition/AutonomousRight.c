//Drive Code for VEX team 2019B, Damage ++ 2017-2018
#include "AutonomousLeft.c";
//VARIABLES


void runAutonRight()
{
	startLift();
	unfoldChainbar();
	mobileGoalDown();
	driveStraight(58.0, 2000);
	mobileGoalUp();
	getCones();
	motor[RLiftMotor] = -maxSpeed;
	motor[LLiftMotor] = -maxSpeed;
	wait1Msec(200);
	motor[RLiftMotor] = 0;
	motor[LLiftMotor] = 0;
	driveStraight(-57.3, 2000);
	LeftDrive(0);
	RightDrive(0);
	wait1Msec(50);
	turn(43, 1, 600);
	driveStraight(-26.0, 1500);
	kP_turn = 0.1;
	turn(104, 1, 600);
	score20();
}
