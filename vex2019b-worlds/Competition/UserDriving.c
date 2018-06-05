int cageHoldPower = 15;
bool cageHold = false;

void arcadeDrive()
{

	int motorPower = vexRT[Ch3];
	int turnPower = vexRT[Ch1];

	//Drive Straight
	if (abs(vexRT[Ch3]) > 45)
	{
		LeftDrive(motorPower);
		RightDrive(motorPower);
	}
	//Turn
	else if (abs(vexRT[Ch1]) > 45)
	{
		RightDrive(-turnPower);
		LeftDrive(turnPower);
	}
	else
	{
		RightDrive(0);
		LeftDrive(0);
	}

	//Goal

	//bring down
	if (vexRT[Btn5D] == 1)
	{
		motor[MGMotor] = maxSpeed;
	}
	//bring up
	else if (vexRT[Btn5U] == 1)
	{
		motor[MGMotor] = -maxSpeed;
	}
	else
	{
		motor[MGMotor] = 0;
	}


	//Cage

	//bring down
	if (vexRT[Btn6D] == 1)
	{
		motor[CageMotor] = maxSpeed;
		cageHold = false;
	}
	if (vexRT[Btn6U] == 1)
	{
		motor[CageMotor] = -maxSpeed;
		cageHold = false;
	}
	if (vexRT[Btn8D] == 1)
	{
		cageHold = true;
	}
	if (vexRT[Btn8U] == 1)
	{
		cageHold = false;
	}
	if (cageHold == false && vexRT[Btn6D] != 1 && vexRT[Btn6U] != 1)
	{
		motor[CageMotor] = 0;
	}

	if (cageHold == true && vexRT[Btn6D] != 1 && vexRT[Btn6U] != 1)
	{
		motor[CageMotor] = cageHoldPower;
	}

}
