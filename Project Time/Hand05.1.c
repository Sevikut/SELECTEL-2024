#pragma config(Sensor, S1,     ,               sensorEV3_Touch)
#pragma config(Sensor, S2,     ,               sensorEV3_Touch)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

int alpha=0, beta=0;
bool a=true;

task preg() {
	float ka=1, kb=1, a=0, b=0, da=1, db=1;
	while(1)
	{
		a=a+sgn(alpha-a)*da;
		b=b+sgn(beta-b)*db;
		motor[motorA]=(a-nMotorEncoder[motorA])*ka;
		motor[motorB]=(b-nMotorEncoder[motorB])*kb;
		wait1Msec(10);
	}
}

task main()
{
	/*motor[motorB] = 20;
	motor[motorD] = 20;
	wait1Msec(2500);
	motor[motorA] = 20;
	wait1Msec(3000);
	motor[motorA] = motor[motorB] = motor[motorD] = 0;*/
	nMotorEncoder[motorA] = 0;
	nMotorEncoder[motorB] = 0;
	startTask(preg);
	while(!SensorValue[S1])
	{
		alpha=0;
		beta=0;
		wait1Msec(2000);
		alpha=180;
		beta=120;
		wait1Msec(2000);
	}
	alpha=0;
	beta=0;
	wait1Msec(100);

	motor[motorD]=-30;
	wait1Msec(1000);

	while (a)
	{
		if (SensorValue[S2])
		{
			a = false;
			wait1Msec(2000);
			nMotorEncoder[motorC]=0;
			while (nMotorEncoder[motorC]>-350)
			{
				motor[motorC]=-40;
			}
			motor[motorC]=0;
		  wait1Msec(5000);
			motor[motorD]=30;
			wait1Msec(5000);
		}
	}
}
