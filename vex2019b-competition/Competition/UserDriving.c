//FORWARD DECLARATIONS
void arcadeDrive();
void controllerTwo();
void controllerOne();
bool intaking = false;
bool mobileIntaking = false;
bool halfSpeed = false;
float limitPower = 3;

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
	if (halfSpeed == false)
	{
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
	}

	if (halfSpeed == true)
	{
		if (abs(vexRT[Ch3]) > 45)
		{
			motor[FLMotor] = motorPower / limitPower;
			motor[BLMotor] = motorPower / limitPower;
			motor[FRMotor] = motorPower / limitPower;
			motor[BRMotor] = motorPower / limitPower;
	  }
	  else if (abs(vexRT[Ch1]) > 45)
		{
			motor[FRMotor] = -turnPower / limitPower;
			motor[BRMotor] = -turnPower / limitPower;
			motor[FLMotor] = turnPower / limitPower;
			motor[BLMotor] = turnPower / limitPower;
	  }
	  else
	  {
	  	motor[FLMotor] = 0;
			motor[BLMotor] = 0;
			motor[FRMotor] = 0;
			motor[BRMotor] = 0;
	  }
	}

	if (vexRT[Btn7U] == 1)
	{
		halfSpeed = true;
	}
	if (vexRT[Btn7D] == 1)
	{
		halfSpeed = false;
	}

  //Goal
  if (vexRT[Btn8L] == 1)
  {
  	mobileIntaking = true;
  }

  if (vexRT[Btn8D] == 1)
	{
		mobileIntaking = false;
		motor[MGMotor] = -maxSpeed;
	}
	if (vexRT[Btn8U] == 1)
	{
		mobileIntaking = false;
		motor[MGMotor] = maxSpeed;
	}

	if (mobileIntaking)
	{
		motor[MGMotor] = -30;
	}

	if (!mobileIntaking && vexRT[Btn8U] != 1 && vexRT[Btn8D] != 1)
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
