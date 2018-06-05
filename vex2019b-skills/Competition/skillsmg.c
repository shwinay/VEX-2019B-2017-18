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

#pragma platform(VEX2)
#pragma competitionControl(Competition)
#include "Vex_Competition_Includes.c"
#include "gyroLib2.c";

//VARIABLES
int maxSpeed = 127;
float kP = .04;
float kI = 0.0001;
float kD = 0;
float kC = 1;

float kP_turn = 1.2;
float kI_turn = .001;

float distance, degrees;
int turnDir;

float error, drivePower, sumError, driftError, driftCorrection;
float MAX_INTEGRAL_VALUE = 50;

//FORWARD DECLARATIONS


//MAIN
void pidTurnLoop(float targetDegrees, int direction);


//TASKS
task main()
{

}

//METHODS
void preauton()
{

}

void pidDriveLoop(float targetTicks)
{
	SensorValue[LeftEncoder] = 0;
	SensorValue[RightEncoder] = 0;
	error = drivePower = sumError = driftError = driftCorrection = integralActiveZone = 0;

	integralActiveZone = targetTicks * .125;

	while (SensorValue[RightEncoder] != targetTicks)
	{
		//Limit Drive Power
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


		if ((sumError * kI) > MAX_INTEGRAL_VALUE))
		{
			sumError = MAX_INTEGRAL_VALUE / kI;
		}

		//Drift Correction
		driftError = SensorValue[RightEncoder] - SensorValue[LeftEncoder];
		driftCorrection = driftError * kC;

		drivePower = kP * error + kI * sumError;

		RightDrive(drivePower - driftCorrection);
		LeftDrive(drivePower + driftCorrection);
	}

	void pidTurnLoop(float targetDegrees, int direction)
	{
		
	}


}
