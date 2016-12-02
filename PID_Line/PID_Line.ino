//PID_Line.ino


#include <Servo.h>

const byte lineSensCount = 11;
const byte lineSensPin[lineSensCount]  =        { A5,   A6,   A7,   A8,   A9,  A10,  A11,  A12,  A13,  A14,  A15};
const byte lineLedPin[lineSensCount]   =        { 35,   36,   37,   38,   39,   40,   41,   42,   43,   44,   45};
int sensitivity=500;

const byte steeringPin = 2;
const int steeringCenter = 1600;
const int steeringLeftMax = 1900;
const int steeringRightMax = 1300;
const int steeringMax = 300;

const byte motorPin = 3;
const int maxSpeed = 1590;//1591
const int minSpeed = 1577
const int zeroSpeed = 1500;
const int fullBrake = 1300;
const int reverseSpeed = 1414;
const int brakeCoef = 15;
const int motorSpeed;

int lineSensValues[lineSensCount];
const int lineSensCoefs[lineSensCount] = {1,	 0.8,	 0.5,	 0.2,	 0.1,	 0 ,	 -0.1,	 -0.2,	 -0.5,	 -0.8,	 -1};
// повороты
const float difTurnCoef = 0.1;
const float proporcTurnCoef = 0.98;
int steeringAngle, steeringAngleOld;
float angleTurn=0, angleTurnOld=0; //pov

int activeSens=0;
int steeringTurn=0;

//время
int timer=0;
int nowTime=0;
int oldTime=0;
int deltaTime=0;

const long serialMonitorSpeed = 115200;
const long serialSirSpeed = 115200;
Servo serv;
Servo dvig;




void setup() {
  // put your setup code here, to run once:
Serial.begin(serialMonitorSpeed);
Serial1.begin(serialSirSpeed);

for (int i=0; i<lineSensCount; i ++)
{
	pinMode(lineLedPin[i], OUTPUT);
    pinMode(lineSensPin[i], INPUT); 
    //lineSensValues[i]=analogRead(lineSensPin[i]);

}
serv.attach(steeringPin);
dvig.attach(motorPin);
serv.writeMicroseconds(steeringCenter);
delay(3000);
dvig.writeMicroseconds(zeroSpeed);
}

void loop() 
{
	drive();
}

void driveNoPID()
{
activeSens=0;
steeringAngle=0;

	for (int i=0; i<lineSensCount; i++)
	{
		pinMode(lineLedPin[i], OUTPUT);
    	pinMode(lineSensPin[i], INPUT); 
    	lineSensValues[i] = analogRead(lineSensPin[i]);
		if (lineSensValues[i] > sensitivity)
		{
			digitalWrite(lineLedPin[i], HIGH);
			steeringAngle = steeringMax*lineSensCoefs[i];
			activeSens+=1;
		}
		else
		{
			digitalWrite(lineLedPin[i], LOW);
		}
	}
angleTurn = steeringAngle/activeSens;
steeringTurn = angleTurn+steeringCenter;
motorSpeed = maxSpeed-(angleTurn/brakeCoef);
serv.writeMicroseconds(steeringTurn);
dvig.writeMicroseconds(motorSpeed);
}

void drivePID()
{
nowTime=millis();
activeSens=0;
steeringAngle=0;

	for (int i=0; i<lineSensCount; i++)
	{
		pinMode(lineLedPin[i], OUTPUT);
    	pinMode(lineSensPin[i], INPUT); 
    	lineSensValues[i] = analogRead(lineSensPin[i]);
		if (lineSensValues[i] > sensitivity)
		{
			digitalWrite(lineLedPin[i], HIGH);
			steeringAngle = steeringMax*lineSensCoefs[i];
			activeSens+=1;
		}
		else
		{
			digitalWrite(lineLedPin[i], LOW);
		}
	}
angleTurn = steeringAngle/activeSens;

deltaTime=nowTime - oldTime;
oldTime = nowTime;
angleTurn = (proporcTurnCoef*angleTurn) + (difTurnCoef*((angleTurn - angleTurnOld)/deltaTime))

steeringTurn = angleTurn+steeringCenter;
motorSpeed = max(maxSpeed-(angleTurn/brakeCoef), minSpeed);
serv.writeMicroseconds(steeringTurn);
dvig.writeMicroseconds(motorSpeed);
}

void IrDa()
{

}
