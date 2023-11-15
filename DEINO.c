/*
 Robot #14
 Usan Siriwardana and Alexander Kuzmenko
 //assuming the user will not stop the robot while turning
*/

void configureAllSensors(); //configures all sensors to standard configuration
void drive(int motor_power); //powers both drive motors with the same power
void driveBoth (int motor_power_B, int motor_power_C); //powers both motors 
void waitButton(TEV3Buttons button_name);  // wait for push and release 

/*
	Improvements made:
	- Gyro does not have to be reset to perform the turn
	- Adjusting feature after performing the turn
*/
int rotateRobot(int angle)
{
	long initialGyro = getGyroDegrees(S4);
	bool buttonPress = false;
	
	if (angle>0)
	{
		driveBoth(-15,15);
	}
	else
	{
		driveBoth(15,-15);
	}
	
	while (abs(getGyroDegrees(S4)-initialGyro)<abs(angle)&& !buttonPress)
	{
		buttonPress = getButtonPress(buttonEnter);
	}
	drive(0);
	
	while(abs(getGyroDegrees(S4)-(initialGyro+angle))!=0&& !buttonPress)
	{
		buttonPress = getButtonPress(buttonEnter);
		
		if(getGyroDegrees(S4)>(initialGyro+angle))
		{
			driveBoth(-5,5);
		}
		else
		{
			driveBoth(5,-5);
		}
	}
	drive(0);
	
	return 2*buttonPress;
}


/*
	Improvements made:
	- nMotorEncoder[motorA] does not need to be reset
	- Slow down feature when close to the desired distance
*/

//Drives robot straight for given distance based on motor encoders and
//nominal wheel diameter. 
//Positive distance is forwardvoid driveDistance(int distance).
int driveDistance (int distance)
{
	long initialEncoder = nMotorEncoder[motorB];
	const float CM_TO_DEG = 180/(2.75*PI);
	bool buttonPress = false;
	bool touchPress = false;
	
	if (distance>0)
	{
		drive(50);
	}
	else
	{
		drive(-50);
	}
	
	while (abs(nMotorEncoder[motorB]-initialEncoder)<abs(distance*CM_TO_DEG)&&
		!buttonPress && !touchPress)
	{
		buttonPress = getButtonPress(buttonEnter);
		touchPress = getTouchValue(S1);
		
		if((abs(distance*CM_TO_DEG)+abs(initialEncoder)) - 
			abs(nMotorEncoder[motorB])< 150)
		{
			if(distance>0)
			{
				drive(10);
			}
			else
			{
				drive(-10);
			}
		}
	}
	
	drive(0);
	
	return touchPress + 2*buttonPress;
}


task main()
{	
	int distanceCheck = 0, rotateCheck = 0;
	
	
	//const int NUM_SIDES = 4;
	const int ANGLE_CORNER = 90;
	const int SIDE_LENGTH = 50;
	const long ENC_TO_DIST = (2.75*PI)/180;
	long distToPress = 0;
	
	configureAllSensors();
	waitButton(buttonEnter);
	
	while(distanceCheck!=2&&rotateCheck!=2)
	{
		//clockwise	reset if looping a second time after touch sensor hit
		distanceCheck = 0;
		rotateCheck = 0;
		
		while(distanceCheck+rotateCheck==0)
		{
			nMotorEncoder[motorC] = 0;
			distanceCheck = driveDistance(SIDE_LENGTH);
			distToPress = nMotorEncoder[motorC]*ENC_TO_DIST;
			
			if(distanceCheck==0)
			{
				rotateCheck = rotateRobot(ANGLE_CORNER);
			}
			else if(distanceCheck == 1)
			{
				rotateRobot(2*ANGLE_CORNER);
				driveDistance(distToPress);
				rotateRobot(-ANGLE_CORNER);
				distToPress = 0;
			}
		}
		
		wait1Msec(1000);
		//counterclockwise
		if(distanceCheck==1)//reset the checkers
		{
			distanceCheck = 0;
			rotateCheck = 0;
			
			while(distanceCheck+rotateCheck==0)
			{
				nMotorEncoder[motorC] = 0;
				distanceCheck = driveDistance(-SIDE_LENGTH);
				distToPress = nMotorEncoder[motorC]*ENC_TO_DIST;
				
				if(distanceCheck==0)
				{
					rotateCheck = rotateRobot(-ANGLE_CORNER);
				}
				else if(distanceCheck == 1)
				{
					rotateRobot(2*ANGLE_CORNER);
					driveDistance(distToPress);
					rotateRobot(ANGLE_CORNER);
					distToPress = 0;
				}
			}
		}
		
	}//exits if Enter pressed
	
	
}
/* ------------------------------------------------------------ */
/* --- Do Not Modify the code below.                        --- */
/* ------------------------------------------------------------ */
void configureAllSensors()
{
	SensorType[S1] = sensorEV3_Touch;
	SensorType[S2] = sensorEV3_Ultrasonic;
	
	SensorMode[S3] = modeEV3Color_Color;
	wait1Msec(50);
	
	SensorType[S4] = sensorEV3_Gyro;
	wait1Msec(50);
	

}
/* DNM: Do Not Modify the code below. */
//powers both drive motors with the same power
void drive(int motor_power)
{
	motor[motorA] = motor[motorD] = motor_power;
}
/* DNM: Do Not Modify the code below. */
//powers both motors independently
void driveBoth(int motor_power_A, int motor_power_D)
{
	motor[motorA] = motor_power_A;
	motor[motorD] = motor_power_D;
}
/* DNM: Do Not Modify the code below. */
void waitButton(TEV3Buttons button_name)
{
	while(!getButtonPress(button_name))
	{	}
	while(getButtonPress(button_name))
	{	}
}
/* ------------------------------------------------------------ */
