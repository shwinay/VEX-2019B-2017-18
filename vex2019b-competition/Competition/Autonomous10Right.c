//Drive Code for VEX team 2019B, Damage ++ 2017-2018
#include "AutonomousLeft.c";
//VARIABLES


void runAuton10Right()
{
	startLift();
	unfoldChainbar();
	mobileGoalDown();
	wait1Msec(50);
	driveStraight(58.0, 2000);
	mobileGoalUp();
	wait1Msec(50);
	getCones();
	wait1Msec(100);
	motor[RLiftMotor] = -maxSpeed;
	motor[LLiftMotor] = -maxSpeed;
	wait1Msec(200);
	motor[RLiftMotor] = 0;
	motor[LLiftMotor] = 0;
	driveStraight(-53.0, 2000);
	LeftDrive(0);
	RightDrive(0);
	wait1Msec(50);
	turn(120, 1, 1200);
	LeftDrive(60);
	RightDrive(60);
	wait1Msec(1050);
	LeftDrive(0);
	RightDrive(0);
	motor[MGMotor] = maxSpeed;
	wait1Msec(400);
	motor[MGMotor] = 0;
	wait1Msec(600);
	LeftDrive(-80);
	RightDrive(-80);
	wait1Msec(500);
	LeftDrive(0);
	RightDrive(0);
}
