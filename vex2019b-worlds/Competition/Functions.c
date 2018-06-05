
#include "GyroLib2.c"

int maxSpeed = 127;

float kP = 0.1;
float kI = 0.00001;//0.00008;//0.0000008;
float kD = 0;//0.3;
float kC = 0.5;//6.5;

float kP_turn = 0.7;
float kI_turn = 0;
float kD_turn = 1.5;

float distance, degrees;
int turnDir;

float error, drivePower, sumError, driftError, driftCorrection;
float MAX_INTEGRAL_VALUE = 50;
float integralActiveZone = 0.0;
float wheelDiameter = 4.0;

float derivative, lastError;
float turnDerivative, lastTurnError;

//methods
void LeftDrive(int power)
{
	motor[FLMotor] = power;
	motor[FMLMotor] = power;
	motor[BMLMotor] = power;
	motor[BLMotor] = power;
}
void RightDrive(int power)
{
	motor[FRMotor] = power;
	motor[FMRMotor] = power;
	motor[BMRMotor] = power;
	motor[BRMotor] = power;
}

void driveTime(int power, int time)
{
	LeftDrive(power);
	RightDrive(power);
	wait1Msec(time);
	LeftDrive(0);
	RightDrive(0);
}

float inchesToTicks(float inches)
{
	return (inches/(4.0*PI))*360.0;
}

void mobileGoalDown(int duration)
{
	motor[MGMotor] = maxSpeed;
	wait1Msec(duration);
	motor[MGMotor] = 0;
}

void mobileGoalUp(int duration)
{
	motor[MGMotor] = -maxSpeed;
	wait1Msec(duration);
	motor[MGMotor] = 0;
}

void cageDown(int duration)
{
	motor[CageMotor] = maxSpeed;
	wait1Msec(duration);
	motor[CageMotor] = 0;
}

void cageUp(int duration)
{
	motor[CageMotor] = -maxSpeed;
	wait1Msec(duration);
	motor[CageMotor] = 0;
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
		//RightDrive(drivePower);
		//LeftDrive(drivePower);

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

task driveDistance
{
	pidDriveLoop(inchesToTicks(distance));
}

task turnDegrees
{
	pidTurnLoop(degrees, turnDir);
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

void letSlack(int dir)
{
	if (dir == 0)
	{
		LeftDrive(40);
		RightDrive(40);
		wait1Msec(300);
	}
	else
	{
		LeftDrive(-40);
		RightDrive(-40);
		wait1Msec(300);
	}

}

void runLeftCage()
{
	kP_turn = 0.4;
	kD_turn = 0.05;
	turn(21, 1, 300);
	letSlack(0);
	driveStraight(52, 2300);
	kP_turn = 0.7;
	kD_turn = 1.5;
	LeftDrive(0);
	RightDrive(0);
	turn(62, 0, 850);
	letSlack(1);
	driveStraight(-56.0, 2300);
	kP_turn = 0.4;
	kD_turn = 0.05;
	turn(10, 0, 300);
	cageDown(1000);
}

void runRightCage()
{
	kP_turn = 0.4;
	kD_turn = 0.05;
	turn(25, 0, 350);
	letSlack(0);
	driveStraight(47, 2200);
	kP_turn = 0.7;
	kD_turn = 1.5;
	turn(72, 1, 850);
	letSlack(1);
	driveStraight(-56.0, 2300);
	kP_turn = 0.4;
	kD_turn = 0.05;
	turn(10, 1, 300);
	cageDown(1300);
}

void runRightDamage()
{
	kP_turn = 0.4;
	kD_turn = 0.05;
	turn(20, 0, 300);
	letSlack(0);
	driveStraight(73, 3000);
	kP_turn = 0.7;
	kD_turn = 1.5;
	turn(123, 1, 1100);
	letSlack(1);
	driveStraight(-70, 2400);
}

void runLeftDamage()
{
	kP_turn = 0.4;
	kD_turn = 0.05;
	turn(15, 1, 300);
	letSlack(0);
	driveStraight(81, 3200);
	kP_turn = 0.7;
	kD_turn = 1.5;
	turn(120, 0, 1100);
	letSlack(1);
	driveStraight(-65, 2400);
}

void runFiveRight()
{
	mobileGoalDown(900);
	wait1Msec(50);
	letSlack(0);
	driveStraight(58.0, 2200);
	mobileGoalUp(1100);
	driveStraight(-37, 1800);
	turn(185, 0, 1300);
	mobileGoalDown(1000);
	driveStraight(-20.0, 1500);
}

void runFiveLeft()
{
	mobileGoalDown(900);
	wait1Msec(50);
	letSlack(0);
	LeftDrive(60);
	RightDrive(60);
	wait1Msec(100);
	driveStraight(58.0, 2200);
	mobileGoalUp(1100);
	driveStraight(-37, 1800);
	turn(185, 1, 1300);
	mobileGoalDown(1000);
	driveStraight(-20.0, 1500);
}

void leftRam()
{
	kP_turn = 0.4;
	kD_turn = 0.05;
	turn(13, 1, 300);
	driveStraight(84, 3000);
}

void rightRam()
{
	kP_turn = 0.4;
	kD_turn = 0.05;
	turn(18, 0, 300);
	driveStraight(84, 3000);
}

void oneCage()
{
	driveStraight(-34, 1500);
	cageDown(1000);
}

void twentyRight()
{
	mobileGoalDown(900);
	wait1Msec(50);
	letSlack(0);
	driveStraight(56.0, 2200);
	mobileGoalUp(950);
	driveStraight(-54, 1800);
	turn(45, 1, 900);
	driveStraight(-26, 1450);
	turn(90, 1, 1100);
	driveTime(maxSpeed, 1000);
	mobileGoalDown(1000);
	driveStraight(-22.0, 1500);
	//turn(90, 0, 1100);
	//driveStraight(20, 800);
	//turn(95, 0, 1100);
	//mobileGoalDown(400);
	//driveStraight(30, 1000);
	//mobileGoalUp(1000);
	//turn(184, 1, 1250);
	//driveStraight(33, 1300);
	//mobileGoalDown(900);
	//driveStraight(-20, 800);
	//turn(184, 0, 1250);
	//mobileGoalDown(300);
	//driveStraight(78, 2500);
	//mobileGoalUp(1000);
	//turn(20, 0, 500);
	//driveStraight(60, 1600);
	//mobileGoalDown(1000);
	//driveStraight(-20, 800);
}

void twentyLeft()
{

}
