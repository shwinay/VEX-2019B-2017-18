#pragma config(Sensor, in8,    gyro,           sensorGyro)
#pragma config(Sensor, dgtl1,  LeftEncoder,    sensorQuadEncoder)
#pragma config(Sensor, dgtl3,  RightEncoder,   sensorQuadEncoder)
#pragma config(Motor,  port1,           MGMotor,       tmotorVex393_HBridge, openLoop)
#pragma config(Motor,  port2,           FLMotor,       tmotorVex393HighSpeed_MC29, openLoop)
#pragma config(Motor,  port3,           BLMotor,       tmotorVex393HighSpeed_MC29, openLoop)
#pragma config(Motor,  port4,           LLiftMotor,    tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port5,           LChainbarMotor, tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port6,           FRMotor,       tmotorVex393HighSpeed_MC29, openLoop, reversed)
#pragma config(Motor,  port7,           BRMotor,       tmotorVex393HighSpeed_MC29, openLoop, reversed)
#pragma config(Motor,  port8,           RLiftMotor,    tmotorVex393_MC29, openLoop, reversed)
#pragma config(Motor,  port9,           RChainbarMotor, tmotorVex393_MC29, openLoop, reversed)
#pragma config(Motor,  port10,          IntakeMotor,   tmotorVex393_HBridge, openLoop)

#include "gyroLib2.c";

//VARIABLES
int maxSpeed = 127;
//float kP = .5;//.5;
//float kI = 0;
//float kD = 5;
//float kC = 1;
float kP = .08; //.057
float kI = 0.0000008; //.000001;
float kD = 0;
float kC = 3.1;//2.8; //1.5

float kP_turn = 1; //0.1
float kI_turn = 0;
float kD_turn = 1.5;//4;

float distance, degrees;
int turnDir;

float error, drivePower, sumError, driftError, driftCorrection;
float MAX_INTEGRAL_VALUE = 50;
float integralActiveZone = 0.0;
float wheelDiameter = 4.0;

float derivative, lastError;
float turnDerivative, lastTurnError;

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

task unfoldChainbarTask
{
	motor[LChainbarMotor] = 60;
	motor[RChainbarMotor] = 60;
	wait1Msec(400);
	motor[LChainbarMotor] = -60;
	motor[RChainbarMotor] = -60;
	wait1Msec(400);
	motor[LChainbarMotor] = 0;
	motor[RChainbarMotor] = 0;
	wait1Msec(50);
	motor[IntakeMotor] = -maxSpeed;
	wait1Msec(80);
	motor[IntakeMotor] = 0;
}

task mgDownTask
{
	mobileGoalDown();
}

task liftUpTask
{
	//bring up lift
	motor[LLiftMotor] = -maxSpeed;
	motor[RLiftMotor] = -maxSpeed;
	wait1Msec(200);
	motor[LLiftMotor] = 0;
	motor[RLiftMotor] = 0;
}
void startLift()
{
	motor[RLiftMotor] = maxSpeed;
	motor[LLiftMotor] = maxSpeed;
	wait1Msec(200);
	motor[RLiftMotor] = 0;
	motor[LLiftMotor] = 0;
	wait1Msec(300);
	motor[RLiftMotor] = -maxSpeed;
	motor[LLiftMotor] = -maxSpeed;
	wait1Msec(320);
	motor[RLiftMotor] = 0;
	motor[LLiftMotor] = 0;
}

//MAIN
void runAutonomous()
{
	//autonomous
	startLift();
	wait1Msec(50);
	kP = .2;
	kD = .3;
	mobileGoalDown();
	driveStraight(58.0, 2000);
	mobileGoalUp();
	driveStraight(-58, 1800);
	LeftDrive(0);
	RightDrive(0);
	wait1Msec(50);
	turn(47.0, 1, 1000);
	wait1Msec(50);
	driveStraight(-25.2, 1200);
	turn(92.0, 1, 1250);
	score20();
	turn(92, 0, 1300);
	wait1Msec(50);
	driveStraight(13, 1100);
	turn(92, 0, 1250);
	wait1Msec(50);
	driveStraight(-10, 1000);
	mobileGoalDown();
	driveStraight(37.0, 1500);
	mobileGoalUp();
	driveStraight(-12.0, 700);
	turn(184, 0, 1500);
	driveStraight(35, 1800);
	mobileGoalDown();
	LeftDrive(0);
	RightDrive(0);
	driveStraight(-11, 800);
	mobileGoalUp();
	LeftDrive(0);
	RightDrive(0);
	wait1Msec(50);
	turn(184, 1, 1500);
	driveStraight(-10, 700);
	mobileGoalDown();
	driveStraight(76, 2000);
	mobileGoalUp();
	driveStraight(23, 1500);
	turn(92, 0, 1250);
	driveStraight(12, 800);
	turn(92, 1, 1250);
	driveStraight(-8, 600);
	score20();
	LeftDrive(-127);
	RightDrive(-127);
	wait1Msec(470);
	LeftDrive(0);
	RightDrive(0);
	wait1Msec(50);
	turn(92, 0, 1250);
	driveStraight(18, 1300);
	turn(92, 0, 1250);
	wait1Msec(50);
	driveStraight(-10, 1000);
	mobileGoalDown();
	driveStraight(37.0, 1500);
	mobileGoalUp();
	driveStraight(-12.0, 700);
	turn(184, 0, 1500);
	driveStraight(35, 1800);
	mobileGoalDown();
	driveStraight(-10, 800);
	mobileGoalUp();
	turn(184, 1, 1500);
	driveStraight(-10, 800);
	mobileGoalDown();
	driveStraight(83, 2600);
	mobileGoalUp();
	driveStraight(21, 1200);
	mobileGoalDown();
	driveStraight(-15, 800);
	mobileGoalUp();

}

task driveDistance
{
	pidDriveLoop(inchesToTicks(distance));
}

task turnDegrees
{
	pidTurnLoop(degrees, turnDir);
}

//METHODS
float inchesToTicks(float inches)
{
	return (inches/(4.0*PI))*360.0;
}

void RightDrive(int speed)
{
	motor[FRMotor] = speed;
	motor[BRMotor] = speed;
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

		if (error == 0)
		{
			derivative = 0;
		}

		//Derivative
		derivative = error - lastError;
		lastError = error;

		//Drift correction
		driftError = SensorValue[RightEncoder] - SensorValue[LeftEncoder];
		driftCorrection = driftError * kC;

		drivePower = kP * error + kI * sumError + kD * derivative;

		RightDrive(drivePower - driftCorrection);
		LeftDrive(drivePower + driftCorrection);
		RightDrive(drivePower);
		LeftDrive(drivePower);

		wait1Msec(25);
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
		while (abs(SensorValue[gyro]) != targetDegrees)
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
			turnDerivative = error - lastTurnError;
			lastTurnError = error;

			//Set motor power
			drivePower = kP_turn * error + kI_turn * sumError + kD_turn * turnDerivative;
			LeftDrive(-drivePower);
			RightDrive(drivePower);

			wait1Msec(25);
		}
	}

	//Turn Right
	if (direction == 1)
	{
		while (abs(SensorValue[gyro]) != targetDegrees)
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
			turnDerivative = error - lastTurnError;
			lastTurnError = error;

			//Set motor power
			drivePower = kP_turn * error + kI_turn * sumError + kD_turn * turnDerivative;
			LeftDrive(drivePower);
			RightDrive(-drivePower);

			wait1Msec(25);
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
	wait1Msec(600);
	motor[MGMotor] = 0;
}

void mobileGoalUp()
{
	motor[MGMotor] = -maxSpeed;
	LeftDrive(25);
	RightDrive(25);
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
	//motor[LLiftMotor] = -127;
	//motor[RLiftMotor] = -127;
	wait1Msec(200);
	//motor[LLiftMotor] = 0;
	//motor[RLiftMotor] = 0;
	wait1Msec(1000);
	//LeftDrive(0);
	//RightDrive(0);
	//wait1Msec(300);
	RightDrive(70);
	LeftDrive(70);
	wait1Msec(300);
	motor[MGMotor] = maxSpeed;
	motor[LChainbarMotor] = -50;
	motor[RChainbarMotor] = -50;
	wait1Msec(300);
	motor[LChainbarMotor] = 0;
	motor[RChainbarMotor] = 0;
	wait1Msec(100);//wait1Msec(300);
	motor[MGMotor] = 0;
	//RightDrive(0);
	//LeftDrive(0);
	//wait1Msec(200);
	LeftDrive(-127);
	RightDrive(-127);
	wait1Msec(480);
	LeftDrive(0);
	RightDrive(0);
	mobileGoalUpNoMg();
}


task main()
{
	//gyro initialize
	GyroInit();
	SensorType[in1] = sensorNone;
	wait1Msec(1000);
	SensorType[in1] = sensorGyro;
	wait1Msec(1000);

	while (true)
	{
		if (vexRT[Btn8U] == 1)
		{
			runAutonomous();
		}
	}
}
