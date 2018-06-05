//Drive Code for VEX team 2019B, Damage ++ 2017-2018
#include "AutonomousLeft.c";
//VARIABLES


void runAuton24Left()
{
	startLift();
	startTask(unfoldChainbarTask);
	startTask(mgDownTask);
	driveStraight(57.0, 2000);
	mobileGoalUp();
	getMoreCones();
	startTask(liftUpTask);
	driveStraight(-36.0, 1600);
	LeftDrive(0);
	RightDrive(0);
	motor[LLiftMotor] = maxSpeed;
	motor[RLiftMotor] = maxSpeed;
	wait1Msec(200);
	motor[LLiftMotor] = 0;
	motor[RLiftMotor] = 0;
	turn(43, 0, 600);
	driveStraight(-27.0, 1500);
	kP_turn = 0.1;
	turn(104, 0, 600);
	score20();
}
