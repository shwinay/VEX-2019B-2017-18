void lcdSetup()
{
  bLCDBacklight = true;
  clearLCDLine(0);
  clearLCDLine(1);
}
void waitForPress()
{
	while(nLCDButtons == 0){}
	wait1Msec(5);
}

//Wait for Release------------------------------------------------
void waitForRelease()
{
	while(nLCDButtons != 0){}
	wait1Msec(5);
}
int count = 0;
const short leftButton = 1;
const short centerButton = 2;
const short rightButton = 4;
void autonSelectUI(){
  	//------------- Start of Kush's OPUI interface Code ---------------------
  	while(nLCDButtons != centerButton)
  	{
  		//Switch case that allows the user to choose from 4 different options
  		switch(count){
  		case 0:
  			//Display first choice
  			displayLCDCenteredString(0, "cageLeft");
  			displayLCDCenteredString(1, "< Enter  >");
  			waitForPress();
  			//Increment or decrement "count" based on button press
  			if(nLCDButtons == leftButton)
  			{
  				waitForRelease();
  				count = 11;
  			}
  			else if(nLCDButtons == rightButton)
  			{
  				waitForRelease();
  				count++;
  			}
  			break;
  		case 1:
  			//Display second choice
  			displayLCDCenteredString(0, "cageRight");
  			displayLCDCenteredString(1, "< Enter  >");
  			waitForPress();
  			//Increment or decrement "count" based on button press
  			if(nLCDButtons == leftButton)
  			{
  				waitForRelease();
  				count--;
  			}
  			else if(nLCDButtons == rightButton)
  			{
  				waitForRelease();
  				count++;
  			}
  			break;
  		case 2:
  			//Display third choice
  			displayLCDCenteredString(0, "fullFieldLeft");
  			displayLCDCenteredString(1, "< Enter  >");
  			waitForPress();
  			//Increment or decrement "count" based on button press
  			if(nLCDButtons == leftButton)
  			{
  				waitForRelease();
  				count--;
  			}
  			else if(nLCDButtons == rightButton)
  			{
  				waitForRelease();
  				count++;
  			}
  			break;
  		case 3:
  			//Display third choice
  			displayLCDCenteredString(0, "fullFieldRight");
  			displayLCDCenteredString(1, "< Enter  >");
  			waitForPress();
  			//Increment or decrement "count" based on button press
  			if(nLCDButtons == leftButton)
  			{
  				waitForRelease();
  				count--;
  			}
  			else if(nLCDButtons == rightButton)
  			{
  				waitForRelease();
  				count++;
  			}
  			break;
  		case 4:
  			//Display fourth choice
  			displayLCDCenteredString(0, "5Left");
  			displayLCDCenteredString(1, "< Enter  >");
  			waitForPress();
  			//Increment or decrement "count" based on button press
  			if(nLCDButtons == leftButton)
  			{
  				waitForRelease();
  				count--;
  			}
  			else if(nLCDButtons == rightButton)
  			{
  				waitForRelease();
  				count++;
  			}
  			break;
  		case 5:
  			//Display fourth choice
  			displayLCDCenteredString(0, "5Right");
  			displayLCDCenteredString(1, "< Enter  >");
  			waitForPress();
  			//Increment or decrement "count" based on button press
  			if(nLCDButtons == leftButton)
  			{
  				waitForRelease();
  				count--;
  			}
  			else if(nLCDButtons == rightButton)
  			{
  				waitForRelease();
  				count++;
  			}
  			break;
  		case 6:
  			//Display fourth choice
  			displayLCDCenteredString(0, "1Cage");
  			displayLCDCenteredString(1, "< Enter  >");
  			waitForPress();
  			//Increment or decrement "count" based on button press
  			if(nLCDButtons == leftButton)
  			{
  				waitForRelease();
  				count--;
  			}
  			else if(nLCDButtons == rightButton)
  			{
  				waitForRelease();
  				count++;
  			}
  			break;
  		case 7:
  			//Display fourth choice
  			displayLCDCenteredString(0, "Left Ram");
  			displayLCDCenteredString(1, "< Enter  >");
  			waitForPress();
  			//Increment or decrement "count" based on button press
  			if(nLCDButtons == leftButton)
  			{
  				waitForRelease();
  				count--;
  			}
  			else if(nLCDButtons == rightButton)
  			{
  				waitForRelease();
  				count++;
  			}
  			break;
		case 8:
			//Display fourth choice
			displayLCDCenteredString(0, "Right Ram");
			displayLCDCenteredString(1, "< Enter  >");
			waitForPress();
			//Increment or decrement "count" based on button press
			if(nLCDButtons == leftButton)
			{
				waitForRelease();
				count--;
			}
			else if(nLCDButtons == rightButton)
			{
				waitForRelease();
				count++;
			}
			break;
		case 9:
			//Display fourth choice
			displayLCDCenteredString(0, "Inspection");
			displayLCDCenteredString(1, "< Enter  >");
			waitForPress();
			//Increment or decrement "count" based on button press
			if(nLCDButtons == leftButton)
			{
				waitForRelease();
				count--;
			}
			else if(nLCDButtons == rightButton)
			{
				waitForRelease();
				count++;
			}
			break;
  		case 10:
  			//Display fourth choice
  			displayLCDCenteredString(0, "20Right");
  			displayLCDCenteredString(1, "< Enter  >");
  			waitForPress();
  			//Increment or decrement "count" based on button press
  			if(nLCDButtons == leftButton)
  			{
  				waitForRelease();
  				count--;
  			}
  			else if(nLCDButtons == rightButton)
  			{
  				waitForRelease();
  				count = 0;
  			}
  			break;

  		default:
  			count = 0;
  			break;
  		}
  	}
  	//------------- End of Kush's OPUI interface Code ---------------------

}
