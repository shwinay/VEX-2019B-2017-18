#pragma config(Sensor, in1,    gyro,           sensorGyro)
#pragma config(Sensor, dgtl1,  RightEncoder,   sensorQuadEncoder)
#pragma config(Sensor, dgtl5,  LeftEncoder,    sensorQuadEncoder)
#pragma config(Motor,  port1,           BLMotor,       tmotorVex393HighSpeed_HBridge, openLoop, reversed)
#pragma config(Motor,  port2,           FLMotor,       tmotorVex393HighSpeed_MC29, openLoop)
#pragma config(Motor,  port3,           FMLMotor,      tmotorVex393HighSpeed_MC29, openLoop)
#pragma config(Motor,  port4,           BMLMotor,      tmotorVex393HighSpeed_MC29, openLoop)
#pragma config(Motor,  port5,           FRMotor,       tmotorVex393HighSpeed_MC29, openLoop, reversed)
#pragma config(Motor,  port6,           FMRMotor,      tmotorVex393HighSpeed_MC29, openLoop, reversed)
#pragma config(Motor,  port7,           BMRMotor,      tmotorVex393HighSpeed_MC29, openLoop, reversed)
#pragma config(Motor,  port8,           MGMotor,       tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port9,           CageMotor,     tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port10,          BRMotor,       tmotorVex393HighSpeed_HBridge, openLoop, reversed)


#pragma systemFile
#pragma platform(VEX2)

#pragma competitionControl(Competition)
#include "Vex_Competition_Includes.c"
#include "GyroLib2.c"
#include "LCD.c"
#include "Functions.c"
#include "UserDriving.c"


void pre_auton()
{
	GyroInit();
	SensorType[in1] = sensorNone;
	wait1Msec(1000);
	SensorType[in1] = sensorGyro;
	wait1Msec(1000);
	lcdSetup();
	autonSelectUI();
}

task autonomous()
{
	switch(count){
	case 0:
		displayLCDCenteredString(0, "Running cageLeft");
		runLeftCage();
		break;
	case 1:
		displayLCDCenteredString(0, "Running cageRight");
		runRightCage();
		break;
	case 2:
		displayLCDCenteredString(0, "Running fullFieldLeft");
		runLeftDamage();
		break;
	case 3:
		displayLCDCenteredString(0, "Running fullFieldRight");
		runRightDamage();
		break;
	case 4:
		displayLCDCenteredString(0, "Running 5Left");
		runFiveLeft();
		break;
	case 5:
		displayLCDCenteredString(0, "Running 5Right");
		runFiveRight();
		break;
	case 6:
		displayLCDCenteredString(0, "Running 1Cage");
		oneCage();
		break;
	case 7:
		displayLCDCenteredString(0, "Running leftRam");
		leftRam();
		break;
	case 8:
		displayLCDCenteredString(0, "Running rightRam");
		rightRam();
		break;
	case 9:
		displayLCDCenteredString(0, "Running Inspection");
		LeftDrive(maxSpeed);
		RightDrive(maxSpeed);
		wait1Msec(1000);
		LeftDrive(0);
		RightDrive(0);
		break;
	case 10:
		displayLCDCenteredString(0, "Running 20Right");
		twentyRight();
		break;
	default:
		displayLCDCenteredString(0, "No valid choice");
		displayLCDCenteredString(1, "was made!");
		break;
	}
}

task usercontrol()
{
  while (true)
  {
		arcadeDrive();
  }
}
