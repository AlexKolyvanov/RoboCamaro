#include <SharpIR.h>
//CamroV3.ino

#include <Servo.h>

const byte lineSensCount = 11;
const byte lineSensPin[lineSensCount]  =        { A5,   A6,   A7,   A8,   A9,  A10,  A11,  A12,  A13,  A14,  A15};
const byte lineLedPin[lineSensCount]   =        { 35,   36,   37,   38,   39,   40,   41,   42,   43,   44,   45};
const float lineSensSensitivity[lineSensCount]= {0.92,   1,	0.94, 0.98,	0.96,	 1,	 1.1, 1.02,	0.94, 1.05,	0.89};

int sensitivity=600;

const byte steeringPin = 2;
const int steeringCenter = 1600;
const int steeringLeftMax = 1900;
const int steeringRightMax = 1300;
const int steeringMax = 300;

const byte motorPin = 3;
const int maxSpeed = 1591;//1591
const int minSpeed = 1577;
const int zeroSpeed = 1500;
const int fullBrake = 1300;
const int reverseSpeed = 1414;
const int brakeCoef = 15;
int motorSpeed;

int lineSensValues[lineSensCount];
const float lineSensCoefs[lineSensCount] = {1,	 0.58,	 0.3,	 0.2,	 0.1,	 0 ,	 -0.1,	 -0.2,	 -0.3,	 -0.58,	 -1};
// повороты
const float difTurnCoef = 0.1;
const float proporcTurnCoef = 0.98;
int steeringAngle, steeringAngleOld;
float angleTurn=0, angleTurnOld=0; //pov

int activeSens=0;
int steeringTurn=0;
int whiteAmount=0;

//время
int timer=0;
int nowTime=0;
int oldTime=0;
int deltaTime=0;

const int frontDictancePin = A4;
int frontDictance = 1000;

const long serialMonitorSpeed = 38400;
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
	activeSens=0;
	steeringAngle=0;

	ReadingSensorsAndLED();
	
	if (activeSens ==0) {dvig.writeMicroseconds(fullBrake); return;}

	DistanceTest();

	//if (frontDictance < 40 ) {dvig.writeMicroseconds(fullBrake); return;}
	
	DriveNoPID();
}

void DriveNoPID()
{
	int gainSteeting=1;
	if (lineSensValues[5] < sensitivity*lineSensSensitivity[6]) gainSteeting = 2;

	angleTurn = (steeringAngle*gainSteeting)/activeSens;
	steeringTurn = angleTurn+steeringCenter;
	motorSpeed = maxSpeed-(angleTurn/brakeCoef);
	Serial.println(motorSpeed);
	serv.writeMicroseconds(steeringTurn);
	dvig.writeMicroseconds(motorSpeed);
}

void WhiteTest()
{
	
}

void DistanceTest()
{
	SharpIR sharp(frontDictancePin, 1080);
	frontDictance=sharp.distance();
	//Serial.println(frontDictance, DEC);
}

void ReadingSensorsAndLED() 
{
	//int SensTest=0;
	for (int i=0; i<lineSensCount; i++)
	{
    	lineSensValues[i] = analogRead(lineSensPin[i]);

    	//Serial.print(lineSensValues[i]*lineSensSensitivity[i]);
    	//Serial.print("  --  ");
		if (lineSensValues[i] > sensitivity*lineSensSensitivity[i])
		{
			digitalWrite(lineLedPin[i], HIGH);
			steeringAngle += steeringMax*lineSensCoefs[i];
			activeSens+=1;
		}
		else
		{
			digitalWrite(lineLedPin[i], LOW);

		}
		
	}
	//Serial.println();
	
	Serial.print(steeringAngle/activeSens);
	Serial.print(' ');
	//Serial.println();
	//SensTest = analogRead(lineSensPin[6]);
	//Serial.println(SensTest, DEC);
}



/*
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



void outputing(linePosition, steeringAngleWheel, speedDrive) //,signalsSvetofor, pedestrian,)  
{
	print(linePosition);
	print(' ');
	print(steeringAngleWheel);
	print(' ');
	println(speedDrive);

}

*/