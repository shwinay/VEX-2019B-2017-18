//Drive Code for VEX team 2019B, Damage ++ 2017-2018
#include "gyroLib2.c";

//VARIABLES
int maxSpeed = 127;
float kP = .08; //.057
float kI = 0.0000008; //.000001;
float kD = 0;
float kC = 3.1;//2.8; //1.5

float kP_turn = 0.2; //0.1
float kI_turn = .0001;
float kD_turn = 0;

float distance, degrees;
int turnDir;

float error, drivePower, sumError, driftError, driftCorrection;
float MAX_INTEGRAL_VALUE = 50;
float integralActiveZone = 0.0;
float wheelDiameter = 4.0;

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
void runAutonLeft();
void getMoreCones();

task driveDistance
{
	pidDriveLoop(inchesToTicks(distance));
}

task turnDegrees
{
	pidTurnLoop(degrees, turnDir);
}

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
	wait1Msec(800);
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
	wait1Msec(300);
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
	motor[LLiftMotor] = -127;
	motor[RLiftMotor] = -127;
	wait1Msec(200);
	motor[LLiftMotor] = 0;
	motor[RLiftMotor] = 0;
	wait1Msec(800);
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
	wait1Msec(300);
	motor[MGMotor] = 0;
	//RightDrive(0);
	//LeftDrive(0);
	//wait1Msec(200);
	LeftDrive(-127);
	RightDrive(-127);
	wait1Msec(300);
	LeftDrive(0);
	RightDrive(0);
	mobileGoalUpNoMg();
}

void getCones()
{
	//drop preload cone
	motor[RLiftMotor] = maxSpeed;
	motor[LLiftMotor] = maxSpeed;
	wait1Msec(300);
	motor[RLiftMotor] = 0;
	motor[LLiftMotor] = 0;
	wait1Msec(300);
	motor[IntakeMotor] = maxSpeed;
	wait1Msec(400);
	motor[IntakeMotor] = 0;
	wait1Msec(50);

	//bring up lift
	motor[LLiftMotor] = -maxSpeed;
	motor[RLiftMotor] = -maxSpeed;
	wait1Msec(80);
	motor[LLiftMotor] = 0;
	motor[RLiftMotor] = 0;
}

void getMoreCones()
{
	//drop preload cone
	motor[RLiftMotor] = maxSpeed;
	motor[LLiftMotor] = maxSpeed;
	wait1Msec(400);
	motor[RLiftMotor] = 0;
	motor[LLiftMotor] = 0;
	wait1Msec(400);
	motor[IntakeMotor] = maxSpeed;
	wait1Msec(400);
	motor[IntakeMotor] = 0;

	//second cone
	motor[RLiftMotor] = maxSpeed;
	motor[LLiftMotor] = maxSpeed;
	wait1Msec(400);
	motor[RLiftMotor] = 0;
	motor[LLiftMotor] = 0;
	motor[LChainbarMotor] = maxSpeed;
	motor[RChainbarMotor] = maxSpeed;
	wait1Msec(200);
	motor[LChainbarMotor] = 40;
	motor[RChainbarMotor] = 40;
	motor[IntakeMotor] = -maxSpeed;
	motor[LLiftMotor] = -maxSpeed;
	motor[RLiftMotor] = -maxSpeed;
	LeftDrive(-60);
	RightDrive(-60);
	wait1Msec(120);
	motor[LLiftMotor] = 0;
	motor[RLiftMotor] = 0;
	wait1Msec(200);
	motor[LChainbarMotor] = maxSpeed;
	motor[RChainbarMotor] = maxSpeed;
	wait1Msec(200);
	motor[LChainbarMotor] = 0;
	motor[RChainbarMotor] = 0;
	wait1Msec(400);
	LeftDrive(0);
	RightDrive(0);
	motor[IntakeMotor] = -80;
	motor[LLiftMotor] = -maxSpeed;
	motor[RLiftMotor] = -maxSpeed;
	wait1Msec(425);
	motor[LLiftMotor] = 0;
	motor[RLiftMotor] = 0;
	motor[LChainbarMotor] = -maxSpeed;
	motor[RChainbarMotor] = -maxSpeed;
	wait1Msec(550);
	motor[LChainbarMotor] = -30;
	motor[RChainbarMotor] = -30;
	motor[LLiftMotor] = maxSpeed;
	motor[RLiftMotor] = maxSpeed;
	wait1Msec(200);
	motor[LLiftMotor] = 0;
	motor[RLiftMotor] = 0;
	//motor[IntakeMotor] = -maxSpeed;
	//wait1Msec(500);
	//motor[IntakeMotor] = 0;
	wait1Msec(230);
	motor[IntakeMotor] = maxSpeed;
	motor[LChainbarMotor] = 0;
	motor[RChainbarMotor] = 0;
	wait1Msec(350);
	motor[IntakeMotor] = 0;
}

void runAutonLeft()
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
	driveStraight(-54, 1800);
	LeftDrive(0);
	RightDrive(0);
	wait1Msec(50);
	turn(43, 0, 600);
	driveStraight(-28.0, 1500);
	kP_turn = 0.1;
	turn(104, 0, 600);
	score20();
}
