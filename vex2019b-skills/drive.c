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

#pragma platform(VEX2)
#pragma competitionControl(Competition)
#include "Vex_Competition_Includes.c"
//VARIABLES
int maxSpeed = 127;
float kP = .04;
float kI = 0.000001;
float kD = 0;
float kC = 1;
float encoderInches = 0;
float inchesToTicks(float inches);
bool infiniteConveyor = false;

float distance, degrees;
int turnDir;

float error, drivePower, sumError, driftError, driftCorrection;
float MAX_INTEGRAL_VALUE = 50;
float integralActiveZone = 0.0;

//FORWARD DECLARATIONS
void arcadeDrive();
void score20MG();
void RightDrive(int speed);
void LeftDrive(int speed);

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


task bringMobileGoalUp
{
	motor[MobileGoalMotor] = -maxSpeed;
	wait1Msec(1410);
	motor[MobileGoalMotor] = 0;
}

task scoreMobileGoalDown
{
	motor[MobileGoalMotor] = maxSpeed;
	wait1Msec(400);
	motor[MobileGoalMotor] = 0;
}
task bringMobileGoalDown
{
	motor[MobileGoalMotor] = maxSpeed;
	wait1Msec(500);
	motor[MobileGoalMotor] = 0;
}

task driveDistance
{
	pidDriveLoop(inchesToTicks(distance));
}

task bringConveyorBack
{
	motor[ConveyorMotor] = maxSpeed;
	wait1Msec(900);
	motor[ConveyorMotor] = 0;
}

task driveMobileGoal
{
	LeftDrive(maxSpeed);
	RightDrive(maxSpeed);
	wait1Msec(600);
	LeftDrive(0);
	RightDrive(0);
}

void pre_auton(){

}

task autonomous()
{
	startTask(bringMobileGoalDown);
	wait1Msec(1000);
	stopTask(bringMobileGoalDown);
	distance = 50.0;
	startTask(driveDistance);
	wait1Msec(2000);
	stopTask(driveDistance);
	LeftDrive(0);
	RightDrive(0);
	startTask(bringMobileGoalUp);
	wait1Msec(1420);
	stopTask(bringMobileGoalUp);
	distance = -42.0;
	startTask(driveDistance);
	wait1Msec(1500);
	motor[ConveyorMotor] = maxSpeed;
	wait1Msec(1200);
	stopTask(driveDistance);
	wait1Msec(1000);
	RightDrive(0);
	LeftDrive(0);
	motor[ConveyorMotor] = 0;
	wait1Msec(500);
	RightDrive(50);
	LeftDrive(50);
	wait1Msec(1000);
	RightDrive(0);
	LeftDrive(0);
}
//MAIN
task usercontrol()
{
	SensorValue[LeftEncoder] = 0;
	SensorValue[RightEncoder] = 0;

	while (true)
	{
		arcadeDrive();
	}
}

//METHODS
void arcadeDrive()
{
	int motorPower = vexRT[Ch3];
	int turnPower = vexRT[Ch1];

	//DRIVE
	if (abs(vexRT[Ch3]) > 45)
	{
		motor[FLDrive] = motorPower;
		motor[MFLDrive] = motorPower;
		motor[MBLDrive] = motorPower;
		motor[BLDrive] = motorPower;

		motor[FRDrive] = motorPower;
		motor[MFRDrive] = motorPower;
		motor[MBRDrive] = motorPower;
		motor[BRDrive] = motorPower;
  }
  else if (abs(vexRT[Ch1]) > 45)
	{                                                                                                                                                                                                                                                                   ;
		motor[FRDrive] = -turnPower;
		motor[MFRDrive] = -turnPower;
		motor[MBRDrive] = -turnPower;
		motor[BRDrive] = -turnPower;

		motor[FLDrive] = turnPower;
		motor[MFLDrive] = turnPower;
		motor[MBLDrive] = turnPower;
		motor[BLDrive] = turnPower;
  }
  else
  {
  	motor[FLDrive] = 0;
		motor[MFLDrive] = 0;
		motor[MBLDrive] = 0;
		motor[BLDrive] = 0;
		motor[FRDrive] = 0;
		motor[MFRDrive] = 0;
		motor[MBRDrive] = 0;
		motor[BRDrive] = 0;
  }

  //MOBILE GOAL
  if (vexRT[Btn5U] == 1)
	{
		motor[MobileGoalMotor] = -maxSpeed;
	}
	else if (vexRT[Btn5D] == 1)
	{
		motor[MobileGoalMotor] = maxSpeed;
	}
	else
	{
		motor[MobileGoalMotor] = 0;
	}

	//CONVEYOR
	if (vexRT[Btn6U] == 1)
	{
		motor[ConveyorMotor] = -maxSpeed;
	}
	else if (vexRT[Btn6D] == 1)
	{
		motor[ConveyorMotor] = maxSpeed;
	}
	else
	{
		motor[ConveyorMotor] = 0;
	}

	//SCORE 20 MOBILEGOAL PRESET
	//if (vexRT[Btn7U] == 1)
	//{
	//	wait1Msec(200);
	//	score20MG();
	//}

	if (vexRT[Btn8U] == 1)
	{
		startTask(bringConveyorBack);
		wait1Msec(900);
		stopTask(bringConveyorBack);
	}

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

float inchesToTicks(float inches)
{
	return ((64.0/36.0)*(inches/(3.25*PI))*360.0);
}