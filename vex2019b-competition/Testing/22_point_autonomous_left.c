#pragma config(Sensor, in8,    gyro,           sensorGyro)
#pragma config(Sensor, dgtl1,  LeftEncoder,    sensorQuadEncoder)
#pragma config(Sensor, dgtl3,  RightEncoder,   sensorQuadEncoder)
#pragma config(Motor,  port1,           MGMotor,       tmotorVex393_HBridge, openLoop)
#pragma config(Motor,  port2,           FLMotor,       tmotorVex393HighSpeed_MC29, openLoop)
#pragma config(Motor,  port3,           BLMotor,       tmotorVex393HighSpeed_MC29, openLoop)
#pragma config(Motor,  port4,           LLiftMotor,    tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port5,           LChainbarMotor, tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port6,           FRMotor,       tmotorVex393HighSpeed_MC29, openLoop)
#pragma config(Motor,  port7,           BRMotor,       tmotorVex393HighSpeed_MC29, openLoop)
#pragma config(Motor,  port8,           RLiftMotor,    tmotorVex393_MC29, openLoop, reversed)
#pragma config(Motor,  port9,           RChainbarMotor, tmotorVex393_MC29, openLoop, reversed)
#pragma config(Motor,  port10,          IntakeMotor,   tmotorVex393_HBridge, openLoop)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

//Drive Code for VEX team 2019B, Damage ++ 2017-2018
#include "gyroLib2.c";

//VARIABLES
int maxSpeed = 127;
float kP = .057;
float kI = 0.0000008; //.000001;
float kD = 0;
float kC = 1.5;

float kP_turn = 0.1;
float kI_turn = .0001;
float kD_turn = 0;

float distance, degrees;
int turnDir;

float error, drivePower, sumError, driftError, driftCorrection;
float MAX_INTEGRAL_VALUE = 50;
float integralActiveZone = 0.0;
float wheelDiameter = 4.0;

float gyroscopeAngle;

//FORWARD DECLARATIONS
float inchesToTicks(float inches);
void RightDrive(int speed);
void LeftDrive(int speed);
void pidDriveLoop(float targetTicks);
void pidTurnLoop(float targetDegrees, int direction);
void driveStraight(float dist, int duration);
void turn(float deg, float dir, int duration);
void mobileGoalDown();
void mobileGoalUp();
void startLift();
void unfoldChainbar();
void score20();
void mobileGoalUpNoMg();
void getCones();
void runAuton();

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
	//initialize gyro
	GyroInit();
	SensorType[in1] = sensorNone;
	wait1Msec(1000);
	SensorType[in1] = sensorGyro;
	wait1Msec(1000);

	while(true)
	{
		if (vexRT[Btn8U] == 1)
		{
			runAuton();
		}
	}
}

//METHODS
float inchesToTicks(float inches)
{
	return (inches/(4.0*PI))*360.0;
}

void RightDrive(int speed)
{
	motor[FRMotor] = -speed;
	motor[BRMotor] = -speed;
}

void LeftDrive(int speed)
{
	motor[FLMotor] = speed;
	motor[BLMotor] = speed;
}

void pidDriveLoop(float targetTicks)
{
	SensorValue[LeftEncoder] = 0;
	SensorValue[RightEncoder] = 0;
	error = drivePower = sumError = driftCorrection = integralActiveZone = 0;

	integralActiveZone = targetTicks * .125;

	while (SensorValue[RightEncoder] != targetTicks)
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
		RightDrive(drivePower);
		LeftDrive(drivePower);
	}
}

void pidTurnLoop(float targetDegrees, int direction)
{
	targetDegrees = targetDegrees * 10.0;
	integralActiveZone = .125 * targetDegrees;

	SensorValue[gyro] = 0;

	//Turn left
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

void driveStraight(float dist, int duration)
{
	distance = dist;
	startTask(driveDistance);
	wait1Msec(duration);
	stopTask(driveDistance);
	LeftDrive(0);
	RightDrive(0);
}

void turn(float deg, float dir, int duration)
{
	degrees = deg;
	turnDir = dir;
	startTask(turnDegrees);
	wait1Msec(duration);
	stopTask(turnDegrees);
	LeftDrive(0);
	RightDrive(0);
}

void mobileGoalDown()
{
	motor[MGMotor] = maxSpeed;
	wait1Msec(500);
	motor[MGMotor] = 0;
}

void mobileGoalUp()
{
	motor[MGMotor] = -maxSpeed;
	LeftDrive(20);
	RightDrive(20);
	wait1Msec(800);
	motor[MGMotor] = 0;
	LeftDrive(0);
	RightDrive(0);
}

void mobileGoalUpNoMg()
{
	motor[MGMotor] = -maxSpeed;
	wait1Msec(400);
	motor[MGMotor] = 0;
}

void startLift()
{
	motor[RLiftMotor] = maxSpeed;
	motor[LLiftMotor] = maxSpeed;
	wait1Msec(200);
	motor[RLiftMotor] = 0;
	motor[LLiftMotor] = 0;
	wait1Msec(600);
	motor[RLiftMotor] = -maxSpeed;
	motor[LLiftMotor] = -maxSpeed;
	wait1Msec(400);
	motor[RLiftMotor] = 0;
	motor[LLiftMotor] = 0;
}

void unfoldChainbar()
{
	motor[LChainbarMotor] = 60;
	motor[RChainbarMotor] = 60;
	wait1Msec(300);
	motor[LChainbarMotor] = 0;
	motor[RChainbarMotor] = 0;
	wait1Msec(200);
	motor[LChainbarMotor] = -60;
	motor[RChainbarMotor] = -60;
	wait1Msec(300);
	motor[LChainbarMotor] = 0;
	motor[RChainbarMotor] = 0;
	wait1Msec(50);
	motor[IntakeMotor] = -maxSpeed;
	wait1Msec(80);
	motor[IntakeMotor] = 0;
}

void score20()
{
	RightDrive(maxSpeed);
	LeftDrive(maxSpeed);
	wait1Msec(1000);
	LeftDrive(0);
	RightDrive(0);
	wait1Msec(300);
	RightDrive(50);
	LeftDrive(50);
	wait1Msec(300);
	motor[LChainbarMotor] = -50;
	motor[RChainbarMotor] = -50;
	wait1Msec(300);
	motor[LChainbarMotor] = 0;
	motor[RChainbarMotor] = 0;
	motor[MGMotor] = maxSpeed;
	wait1Msec(500);
	motor[MGMotor] = 0;
	RightDrive(0);
	LeftDrive(0);
	wait1Msec(200);
	LeftDrive(-127);
	RightDrive(-127);
	wait1Msec(700);
	LeftDrive(0);
	RightDrive(0);
	mobileGoalUpNoMg();
}

void getCones()
{
	motor[RLiftMotor] = maxSpeed;
	motor[LLiftMotor] = maxSpeed;
	wait1Msec(200);
	motor[RLiftMotor] = 0;
	motor[LLiftMotor] = 0;
	wait1Msec(200);
	motor[IntakeMotor] = maxSpeed;
	wait1Msec(700);
	motor[IntakeMotor] = 0;
	wait1Msec(200);
	//second cone
	//motor[LChainbarMotor] = maxSpeed;
	//motor[RChainbarMotor] = maxSpeed;
	//wait1Msec(200);
	//motor[LChainbarMotor] = 0;
	//motor[RChainbarMotor] = 0;
	//motor[IntakeMotor] = -maxSpeed;
	//wait1Msec(1000);
	//motor[IntakeMotor] = 0;
	motor[LLiftMotor] = -maxSpeed;
	motor[RLiftMotor] = -maxSpeed;
	wait1Msec(400);
	motor[LLiftMotor] = 0;
	motor[RLiftMotor] = 0;
	//motor[LChainbarMotor] = -maxSpeed;
	//motor[RChainbarMotor] = -maxSpeed;
	//wait1Msec(500);
	//motor[LChainbarMotor] = 0;
	//motor[RChainbarMotor] = 0;
	//wait1Msec(300);
	//motor[IntakeMotor] = maxSpeed;
	//wait1Msec(800);
	//motor[IntakeMotor] = 0;
}

void runAuton()
{
	startLift();
	wait1Msec(200);
	unfoldChainbar();
	mobileGoalDown();
	wait1Msec(200);
	driveStraight(53.0, 2000);
	mobileGoalUp();
	wait1Msec(200);
	getCones();
	wait1Msec(100);
	driveStraight(-52.0, 2000);
	turn(57, 0, 600);
	driveStraight(-32.0, 1750);
	turn(104, 0, 600);
	score20();
}
