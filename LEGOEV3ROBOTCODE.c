// Colour is port one, gyro is sensor 4, ultraonis is 2,
// Motor A and D are for steering, Motor B is for lowering the claw, Motor C is the claw motor


float detectShortest(){
	resetGyro(S4);
	motor[motorA] = 5;
	motor[motorD] = -5;

	float shortestDist = 10000;

	while (getGyroDegrees(S4) < 360)
	{
		if(shortestDist > SensorValue[S2])
		{
			shortestDist = SensorValue[S2];
		}
		wait1Msec(150);
	}
	motor[motorA] = motor[motorD] = 0;
	return shortestDist;
}

float driveToObject(float shortestDist){
	float convertedDist = (shortestDist-15)*180/PI/2.75;
	motor[motorA] = 5;
	motor[motorD] = -5;

	resetGyro(S4);

	while(SensorValue[S2] != shortestDist)
	{}

	motor[motorA] = motor[motorD] = 0;
	wait1Msec(150);
	nMotorEncoder[motorA] = 0;
	motor[motorA] = motor[motorD] = 20;

	while(nMotorEncoder[motorA] < convertedDist)
	{}
	float angle = getGyroDegrees(S4);
	motor[motorA] = motor[motorD] = 0;
	return angle;
}

void configureAllSensors(){
	SensorType[S1] = sensorEV3_Color;
	wait1Msec(50);
	SensorMode[S1] = modeEV3Color_Color;
	wait1Msec(50);
	SensorType[S2] = sensorEV3_Ultrasonic;
	SensorType[S4] = sensorEV3_Gyro;
	wait1Msec(50);
	SensorMode[S4] = modeEV3Gyro_Calibration;
	wait1Msec(50);
	SensorMode[S4] = modeEV3Gyro_RateAndAngle;
}

bool pickUp(){
	nMotorEncoder[motorA] = 0;
	motor[motorA] = motor[motorD] = -20;
	while(abs(nMotorEncoder[motorA]) < 10.5*180/2.75/PI)
	{}
	motor[motorA] = motor[motorD] = 0;
	motor[motorB] = -75;
	wait1Msec(2800);
	motor[motorB] = 0;
	wait1Msec(2000);
	motor[motorC] = -25;
	wait1Msec(1500);
	motor[motorC] = 0;
	int colour = SensorValue[S1];
	bool isObjectRed = false;
	wait1Msec(2500);
	if (colour == (int)colorRed)
	{
		isObjectRed = true;
	}
	if (isObjectRed)
	{
		motor[motorB] = 75;
		wait1Msec(3750);
		motor[motorB] = 0;
	}
	if (!isObjectRed)
	{
		motor[motorC] = 25;
		wait1Msec(1500);
		motor[motorC] = 0;
		motor[motorB] = 75;
		wait1Msec(3750);
		motor[motorB] = 0;
	}
	return isObjectRed;
}

void dropObject(bool isObjectRed, float shortestDist){
		if(isObjectRed)
		{
			nMotorEncoder[motorA] = 0;
			motor[motorA] = motor[motorD] = 25;
			while (nMotorEncoder[motorA] < 250*180/PI/2.75)
			{}
			motor[motorA] = motor[motorD] = 0;
			motor[motorC] = 25;
			wait1Msec(1500);
			motor[motorC] = 0;
			motor[motorB] = -75;
			wait1Msec(2800);
			motor[motorB] = 0;
			motor[motorB] = 75;
			wait1Msec(3750);
			motor[motorB] = 0;
		}
}

void returnOriginal(bool isObjectRed, float angle)
{
	if (isObjectRed)
	{
		motor[motorA] = motor[motorD] = -25;
		nMotorEncoder[motorA] = 0;
		while(abs(nMotorEncoder[motorA]) < 250*180/PI/2.75)
		{}
		resetGyro(S4);
		motor[motorA] = 5;
		motor[motorD] = -5;
		wait1Msec(4000);
		while(SensorValue(S4) < (360 - angle))
		{}
		motor[motorA] = motor[motorD] = 0;
		wait1Msec(4000);
		nMotorEncoder[motorA] = 0;
		resetGyro(S4);
	}
}


task main()
{
	configureAllSensors();
	bool isObjectRed = true;
	//motor[motorB] = 50;
	//wait1Msec(1500);
	//motor[motorB] = 0;
	while (isObjectRed){
		float shortestDist = detectShortest();
		float angle = driveToObject(shortestDist);
		isObjectRed = pickUp();
		dropObject(isObjectRed, shortestDist);
		returnOriginal(isObjectRed, angle);
	}
}
