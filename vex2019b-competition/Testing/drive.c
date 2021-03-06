#pragma config(Sensor, in1,    gyro,           sensorGyro)
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
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

//Drive Code for VEX team 2019B, Damage ++ 2017-2018


//VARIABLES
int maxSpeed = 127;

//FORWARD DECLARATIONS
void arcadeDrive();
void controllerTwo();
void controllerOne();
bool intaking = false;

//MAIN
task main()
{
	while (true)
	{
		arcadeDrive();
	}
}

//METHODS
void arcadeDrive()
{
	controllerOne();
	controllerTwo();
}

void controllerOne()
{
	int motorPower = vexRT[Ch3];
	int turnPower = vexRT[Ch1];

	//Drive Straight
	if (abs(vexRT[Ch3]) > 45)
	{
		motor[FLMotor] = motorPower;
		motor[BLMotor] = motorPower;
		motor[FRMotor] = motorPower;
		motor[BRMotor] = motorPower;
  }
  else if (abs(vexRT[Ch1]) > 45)
	{
		motor[FRMotor] = -turnPower;
		motor[BRMotor] = -turnPower;
		motor[FLMotor] = turnPower;
		motor[BLMotor] = turnPower;
  }
  else
  {
  	motor[FLMotor] = 0;
		motor[BLMotor] = 0;
		motor[FRMotor] = 0;
		motor[BRMotor] = 0;
  }


  //Goal
  if (vexRT[Btn8D] == 1)
	{
		motor[MGMotor] = maxSpeed;
	}
	else if (vexRT[Btn8U] == 1)
	{
		motor[MGMotor] = -maxSpeed;
	}
  else
  {
  	motor[MGMotor] = 0;
  }
}

void controllerTwo()
{
	int liftPower = vexRT[Ch3Xmtr2];

	//Lift
	if (abs(vexRT[Ch3Xmtr2]) > 45)
	{
		motor[RLiftMotor] = -liftPower;
		motor[LLiftMotor] = -liftPower;
  }
  else
  {
  	motor[RLiftMotor] = 0;
  	motor[LLiftMotor] = 0;
  }

  int chainbarPower = vexRT[Ch2Xmtr2];

  //Chainbar
  if (abs(vexRT[Ch2Xmtr2]) > 45)
  {
		motor[LChainbarMotor] = -chainbarPower;
		motor[RChainbarMotor] = -chainbarPower;
  }
  else
  {
  	motor[LChainbarMotor] = 0;
  	motor[RChainbarMotor] = 0;
  }


  //Intake
  if (vexRT[Btn5UXmtr2] == 1)
  {
  	motor[IntakeMotor] = -maxSpeed;
  	intaking = true;
  }
  if (vexRT[Btn5DXmtr2] == 1)
  {
  	motor[IntakeMotor] = maxSpeed;
  	intaking = true;
  }
  if (vexRT[Btn6UXmtr2] == 1)
  {
  	intaking = false;
  }

  if (intaking == false)
  {
  	motor[IntakeMotor] = 0;
  }

}
