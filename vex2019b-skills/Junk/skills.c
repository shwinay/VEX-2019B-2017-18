#pragma config(Sensor, in1,    gyro,           sensorGyro)
#pragma config(Sensor, dgtl1,  LeftEncoder,    sensorQuadEncoder)
#pragma config(Sensor, dgtl3,  RightEncoder,   sensorQuadEncoder)
#pragma config(Motor,  port1,           MBRDrive,      tmotorVex393HighSpeed_HBridge, openLoop)
#pragma config(Motor,  port2,           FLDrive,       tmotorVex393HighSpeed_MC29, openLoop)
#pragma config(Motor,  port3,           MFLDrive,      tmotorVex393HighSpeed_MC29, openLoop, reversed)
#pragma config(Motor,  port4,           MBLDrive,      tmotorVex393HighSpeed_MC29, openLoop, reversed)
#pragma config(Motor,  port5,           BLDrive,       tmotorVex393HighSpeed_MC29, openLoop)
#pragma config(Motor,  port6,           FRDrive,       tmotorVex393HighSpeed_MC29, openLoop, reversed)
#pragma config(Motor,  port7,           MFRDrive,      tmotorVex393HighSpeed_MC29, openLoop)
#pragma config(Motor,  port8,           ConveyorMotor, tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port9,           MobileGoalMotor, tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port10,          BRDrive,       tmotorVex393HighSpeed_HBridge, openLoop, reversed)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#include "gyroLib2.c";

//VARIABLES
int maxSpeed = 127;
float kP = .04;
float kI = 0.000001;
float kD = 0;
float kC = 1;

float kP_turn = 1.2;
float kI_turn = .001;
float kD_turn = 0;

float distance, degrees;
int turnDir;

float error, drivePower, sumError, driftError, driftCorrection;
float MAX_INTEGRAL_VALUE = 50;
float integralActiveZone = 0.0;

//FORWARD DECLARATIONS
float inchesToTicks(float inches);
void RightDrive(int speed);
void LeftDrive(int speed);
void pidDriveLoop(float targetTicks);
void pidTurnLoop(float targetDegrees, int direction);
void goHalfway();
void score20MG();
void score10MG();
void repositionRobot();
void fifthMG();
void goAnotherHalfway();
void scoreAnother20MG();


task bringMobileGoalDown
{
	motor[MobileGoalMotor] = maxSpeed;
	wait1Msec(500);
	motor[MobileGoalMotor] = 0;
}

task scoreMobileGoalDown
{
	motor[MobileGoalMotor] = maxSpeed;
	wait1Msec(400);
	motor[MobileGoalMotor] = 0;
}


task bringMobileGoalUp
{
	motor[MobileGoalMotor] = -maxSpeed;
	wait1Msec(1410);
	motor[MobileGoalMotor] = 0;
}

task initialDrive
{
	pidDriveLoop(inchesToTicks(30.0));
}

task secondaryDrive
{
	pidDriveLoop(inchesToTicks(40.0));
}

task tertiaryDrive
{
	//pidDriveLoop(inchesToTicks(29.0));
	pidDriveLoop(inchesToTicks(31.0));
}

task quadraryDrive
{
	pidDriveLoop(inchesToTicks(25.0));
}

task fifthDrive()
{
	//pidDriveLoop(-inchesToTicks(17.0));
	pidDriveLoop(-inchesToTicks(17.0));
}

task sixthDrive()
{
	LeftDrive(-50);
	RightDrive(-50);
	wait1Msec(350);
	RightDrive(0);
	LeftDrive(0);
}

task driveMobileGoal
{
	LeftDrive(maxSpeed);
	RightDrive(maxSpeed);
	wait1Msec(600);
	LeftDrive(0);
	RightDrive(0);
}

task bringConveyorBack
{
	motor[ConveyorMotor] = maxSpeed;
	wait1Msec(1050);
	motor[ConveyorMotor] = 0;
}

task firstTurn
{
	//kP_turn = 2.0;
	pidTurnLoop(24.0, 0);
}

task secondTurn
{
	//kP_turn = 2.0;
	pidTurnLoop(24.0, 1);
}

task thirdTurn
{
	pidTurnLoop(65.0, 0);
}


task seventhDrive
{
	pidDriveLoop(inchesToTicks(23.0));
}

task eighthDrive()
{
	LeftDrive(-50);
	RightDrive(-50);
	wait1Msec(490);
	RightDrive(0);
	LeftDrive(0);
}

task driveDistance
{
	pidDriveLoop(inchesToTicks(distance));
}

task turnDegrees
{
	pidTurnLoop(degrees, turnDir);
}

//MAIN
task main()
{
	GyroInit();
	SensorType[in1] = sensorNone;
	wait1Msec(1000);
	SensorType[in1] = sensorGyro;
	wait1Msec(1000);
	//////////////////////////////

	goHalfway();
	repositionRobot();
	goAnotherHalfway();
	wait1Msec(200);
	fifthMG();

	/////////////////////////////
	wait1Msec(10000);
}

//METHODS
float inchesToTicks(float inches)
{
	return ((64.0/36.0)*(inches/(3.25*PI))*360.0);
}

void RightDrive(int speed)
{
	motor[FRDrive] = speed;
	motor[MFRDrive] = speed;
	motor[MBRDrive] = speed;
	motor[BRDrive] = speed;
}

void LeftDrive(int speed)
{
	motor[FLDrive] = speed;
	motor[MFLDrive] = speed;
	motor[MBLDrive] = speed;
	motor[BLDrive] = speed;
}
void pidDriveLoop(float targetTicks)
{
	SensorValue[LeftEncoder] = 0;
	SensorValue[RightEncoder] = 0;
	error = drivePower = sumError = driftError = driftCorrection = integralActiveZone = 0;
	integralActiveZone = targetTicks * .125;

	while(SensorValue[RightEncoder] != targetTicks)
	{
		//Limit drive power
		if(drivePower > maxSpeed)
		{
			drivePower = maxSpeed;
		}
		if(drivePower < -maxSpeed)
		{
			drivePower = -maxSpeed;
		}

		//Proportional
		error = targetTicks - SensorValue[RightEncoder];

		//Integral
		if (abs(error) < inchesToTicks(integralActiveZone))
		{
			sumError += error;
		}
		else
		{
			sumError = 0;
		}

		if ((sumError*kI) > MAX_INTEGRAL_VALUE)
		{
			sumError = MAX_INTEGRAL_VALUE / kI;
		}

		//Derivative

		//Drift correction
		driftError = SensorValue[RightEncoder] - SensorValue[LeftEncoder];
		driftCorrection = driftError * kC;

		drivePower = kP * error + kI * sumError;

		RightDrive(drivePower - driftCorrection);
		LeftDrive(drivePower + driftCorrection);
	}
}

void pidTurnLoop(float targetDegrees, int direction)
{
	targetDegrees = targetDegrees * 10.0;
	integralActiveZone = .125 * targetDegrees;

	SensorValue[gyro] = 0;

	//Turn Left
	if (direction == 0)
	{
		while (abs(SensorValue[gyro]) < targetDegrees)
		{
			//Limit drive power
			if (drivePower > maxSpeed)
			{
				drivePower = maxSpeed;
			}
			if (drivePower < -maxSpeed)
			{
				drivePower = -maxSpeed;
			}

			//Proportional
			error = targetDegrees - abs(SensorValue[gyro]);

			//Integral
			if (error < integralActiveZone)
			{
				sumError += error;
			}
			if (error > integralActiveZone)
			{
				sumError = 0;
			}
			if ((sumError * kI_turn) > MAX_INTEGRAL_VALUE)
			{
				sumError = MAX_INTEGRAL_VALUE / kI_turn;
			}

			//Derivative

			//Set motor power
			drivePower = kP_turn * error + kI_turn * sumError;
			LeftDrive(-drivePower);
			RightDrive(drivePower);

		}
	}

	//Turn Right
	if (direction == 1)
	{
		while (abs(SensorValue[gyro]) < targetDegrees)
		{
			//Limit drive power
			if (drivePower > maxSpeed)
			{
				drivePower = maxSpeed;
			}
			if (drivePower < -maxSpeed)
			{
				drivePower = -maxSpeed;
			}

			//Proportional
			error = targetDegrees - abs(SensorValue[gyro]);

			//Integral
			if (error < integralActiveZone)
			{
				sumError += error;
			}
			if (error > integralActiveZone)
			{
				sumError = 0;
			}
			if ((sumError * kI_turn) > MAX_INTEGRAL_VALUE)
			{
				sumError = MAX_INTEGRAL_VALUE / kI_turn;
			}

			//Derivative

			//Set motor power
			drivePower = kP_turn * error + kI_turn * sumError;
			LeftDrive(drivePower);
			RightDrive(-drivePower);
		}
	}
}

void goHalfway()
{
	startTask(bringMobileGoalDown);
	wait1Msec(1000);
	stopTask(bringMobileGoalDown);
	startTask(initialDrive);
	wait1Msec(1700);
	stopTask(initialDrive);
	LeftDrive(0);
	RightDrive(0);
	startTask(bringMobileGoalUp);
	wait1Msec(1000);
	startTask(bringConveyorBack);
	wait1Msec(1500);
	startTask(bringMobileGoalDown);
	wait1Msec(1500);
	startTask(secondaryDrive);
	wait1Msec(2000);
	stopTask(secondaryDrive);
	LeftDrive(0);
	RightDrive(0);
	startTask(bringMobileGoalUp);
	wait1Msec(1500);
	startTask(tertiaryDrive);
	wait1Msec(2200);
	stopTask(tertiaryDrive);
	LeftDrive(0);
	RightDrive(0);
	startTask(firstTurn);
	wait1Msec(500);
	stopTask(firstTurn);
	startTask(quadraryDrive);
	wait1Msec(2100);
	stopTask(quadraryDrive);
	startTask(secondTurn);
	wait1Msec(600);
	stopTask(secondTurn);
	LeftDrive(0);
	RightDrive(0);
	startTask(fifthDrive);
	wait1Msec(1000);
	stopTask(fifthDrive);
	LeftDrive(0);
	RightDrive(0);
	score20MG();
	score10MG();
}

void goAnotherHalfway()
{
	startTask(bringMobileGoalDown);
	wait1Msec(1000);
	stopTask(bringMobileGoalDown);
	startTask(initialDrive);
	wait1Msec(1700);
	stopTask(initialDrive);
	LeftDrive(0);
	RightDrive(0);
	startTask(bringMobileGoalUp);
	wait1Msec(1000);
	startTask(bringConveyorBack);
	wait1Msec(1500);
	startTask(bringMobileGoalDown);
	wait1Msec(1500);
	startTask(secondaryDrive);
	wait1Msec(2000);
	stopTask(secondaryDrive);
	LeftDrive(0);
	RightDrive(0);
	startTask(bringMobileGoalUp);
	wait1Msec(1500);
	startTask(tertiaryDrive);
	wait1Msec(2200);
	stopTask(tertiaryDrive);
	LeftDrive(0);
	RightDrive(0);
	startTask(firstTurn);
	wait1Msec(500);
	stopTask(firstTurn);
	startTask(quadraryDrive);
	wait1Msec(2100);
	stopTask(quadraryDrive);
	startTask(secondTurn);
	wait1Msec(600);
	stopTask(secondTurn);
	LeftDrive(0);
	RightDrive(0);
	startTask(fifthDrive);
	wait1Msec(1000);
	stopTask(fifthDrive);
	LeftDrive(0);
	RightDrive(0);
	scoreAnother20MG();
	score10MG();
}


void score20MG()
{
	startTask(scoreMobileGoalDown);
	wait1Msec(500);
	stopTask(scoreMobileGoalDown);
	startTask(driveMobileGoal);
	wait1Msec(400);
	startTask(scoreMobileGoalDown);
	wait1Msec(300);
	stopTask(scoreMobileGoalDown);
	stopTask(driveMobileGoal);
	wait1Msec(500);
	LeftDrive(0);
	RightDrive(0);
}

void scoreAnother20MG()
{
	startTask(scoreMobileGoalDown);
	wait1Msec(500);
	stopTask(scoreMobileGoalDown);
	startTask(driveMobileGoal);
	wait1Msec(325);
	startTask(scoreMobileGoalDown);
	wait1Msec(300);
	stopTask(scoreMobileGoalDown);
	stopTask(driveMobileGoal);
	wait1Msec(500);
	LeftDrive(0);
	RightDrive(0);
}

void score10MG()
{
	motor[MobileGoalMotor] = -maxSpeed;
	wait1Msec(800);
	motor[MobileGoalMotor] = 0;
	wait1Msec(500);
	motor[ConveyorMotor] = -maxSpeed;
	wait1Msec(1500);
	motor[ConveyorMotor] = 0;
}

void repositionRobot()
{
	wait1Msec(500);
	startTask(sixthDrive);
	wait1Msec(700);
	stopTask(sixthDrive);
	startTask(thirdTurn);
	wait1Msec(700);
	stopTask(thirdTurn);
	LeftDrive(0);
	RightDrive(0);
	startTask(seventhDrive);
	wait1Msec(1400);
	stopTask(seventhDrive);
	LeftDrive(0);
	RightDrive(0);
	wait1Msec(200);
	startTask(thirdTurn);
	wait1Msec(700);
	stopTask(thirdTurn);
	LeftDrive(0);
	RightDrive(0);
	startTask(eighthDrive);
	wait1Msec(500);
	stopTask(eighthDrive);
	LeftDrive(0);
	RightDrive(0);
	wait1Msec(200);
}

void fifthMG()
{
	distance = -15.0;
	startTask(driveDistance);
	wait1Msec(700);
	stopTask(driveDistance);
	LeftDrive(0);
	RightDrive(0);
	wait1Msec(200);
	degrees = 62.0;
	turnDir = 1;
	startTask(turnDegrees);
	wait1Msec(700);
	stopTask(turnDegrees);
	RightDrive(0);
	LeftDrive(0);
	wait1Msec(200);
	distance = 31.0;
	startTask(driveDistance);
	wait1Msec(2000);
	stopTask(driveDistance);
	wait1Msec(300);
	degrees = 16;
	turnDir = 1;
	startTask(turnDegrees);
	wait1Msec(500);
	stopTask(turnDegrees);
	LeftDrive(0);
	RightDrive(0);
	startTask(bringMobileGoalDown);
	wait1Msec(1000);
	stopTask(bringMobileGoalDown);
	distance = 35.0;
	startTask(driveDistance);
	wait1Msec(2000);
	stopTask(driveDistance);
	LeftDrive(0);
	RightDrive(0);
	startTask(bringMobileGoalUp);
	wait1Msec(1420);
	stopTask(bringMobileGoalUp);
	distance = -45.0;
	startTask(driveDistance);
	wait1Msec(1300);
	motor[ConveyorMotor] = maxSpeed;
	wait1Msec(1200);
	stopTask(driveDistance);
	LeftDrive(0);
	RightDrive(0);
	RightDrive(-70);
	wait1Msec(1000);
	RightDrive(0);
	wait1Msec(500);
	motor[ConveyorMotor] = 0;
	wait1Msec(200);
	LeftDrive(30);
	RightDrive(30);
	wait10Msec(50);
	LeftDrive(0);
	RightDrive(0);
	//LeftDrive(-127);
	//RightDrive(-127);
	//wait1Msec(300);
	//LeftDrive(0);
	//RightDrive(0);
}
