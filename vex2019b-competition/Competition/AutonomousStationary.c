//Drive Code for VEX team 2019B, Damage ++ 2017-2018
#include "AutonomousLeft.c";
//VARIABLES


void runAutonStationary()
{
	//Prepare Lift
	startLift();
	unfoldChainbar();
	//mobileGoalDown();
	wait1Msec(200);
	motor[LLiftMotor] = -maxSpeed;
	motor[RLiftMotor] = -maxSpeed;
	wait1Msec(300);
	motor[LLiftMotor] = 0;
	motor[RLiftMotor] = 0;
	//Stationary Auton
	driveStraight(24.0, 1500);
	motor[LLiftMotor] = maxSpeed;
	motor[RLiftMotor] = maxSpeed;
	wait1Msec(300);
	motor[LLiftMotor] = 0;
	motor[LLiftMotor] = 0;
	motor[IntakeMotor] = maxSpeed;
	wait1Msec(400);
	motor[IntakeMotor] = 0;
	motor[LLiftMotor] = -maxSpeed;
	motor[RLiftMotor] = -maxSpeed;
	wait1Msec(300);
	motor[LLiftMotor] = 0;
	motor[RLiftMotor] = 0;
	LeftDrive(-80);
	RightDrive(-80);
	wait1Msec(800);
	LeftDrive(0);
	RightDrive(0);

}
