//CamaroV5.ino
#include <SharpIR.h>
//CamroV4.ino

#include <Servo.h>

const byte lineSensCount = 11;
const byte lineSensPin[lineSensCount]  =        { A5,   A6,   A7,   A8,   A9,  A10,  A11,  A12,  A13,  A14,  A15};
const byte lineLedPin[lineSensCount]   =        { 35,   36,   37,   38,   39,   40,   41,   42,   43,   44,   45}; //left - right
const float lineSensSensitivity[lineSensCount]= {552,  600,	 564,  588,	 576,  600,	 660,  612,	 564,  588,	 534};

int sensitivity=1;  // Увеличивай - если в белом видит черное

const byte steeringPin = 2;
const int steeringCenter = 1600;
const int steeringLeftMax = 1900;
const int steeringRightMax = 1300;
const int steeringMax = 300;

const byte motorPin = 3;
const int maxSpeed = 1591;//1591
const int minSpeed = 1578;
const int zeroSpeed = 1500;
const int fullBrake = 1300;
const int reverseSpeed = 1414;
const int brakeCoef = 15;
const int svetoforSpeed = 1582;
int motorSpeed, factMotorSpeed;
int whiteTester;

int lineSensValues[lineSensCount];
char charSensValues[lineSensCount-1];
const float lineSensCoefs[lineSensCount] = {1,	 0.58,	 0.3,	 0.2,	 0.1,	 0,	 -0.1,	 -0.2,	 -0.3,	 -0.58,	 -1};
const float linePropCoefs[lineSensCount] = {1,   0.8,    0.6,    0.4,    0.2,    0,  -0.2,   -0.4,   -0.6,   -0.8,   -1};
int linePosition=0;
// повороты
const float difTurnCoef = 0.1;
const float proporcTurnCoef = 0.98;
int steeringAngle, steeringAngleOld;
float angleTurn=0, angleTurnOld=0; //pov

int activeSens=0;
int steeringTurn=0;
int whiteAmount=0;
int crossSens=0;

//время
unsigned long time, oldTime=0;

int svetSignal =0;
int svetFlag=0;

const int frontDictancePin = A4;
int frontDictance = 1000;
boolean pedestrian=0;
int selfDistance =20;

const long serialMonitorSpeed = 38400;
const long serialSirSpeed = 9600;
Servo serv;
Servo dvig;




void setup() {
  // put your setup code here, to run once:
	Serial.begin(serialMonitorSpeed);
	Serial1.begin(serialSirSpeed);
	PinModing();
	serv.attach(steeringPin);
	dvig.attach(motorPin);
	steeringTurn = steeringCenter;
	serv.writeMicroseconds(steeringTurn);
	motorSpeed = zeroSpeed;
	dvig.writeMicroseconds(motorSpeed);
	delay(3000);
	motorSpeed = zeroSpeed;
	motorSpeed=CalculationSpeed();
}

void loop() 
{
	
	activeSens=0;
	steeringAngle=0;

	angleTurn = ReadingSensorsAndLED();
	
	serv.writeMicroseconds(angleTurn+steeringCenter);

	whiteTester = WhiteTest(); 

	frontDictance = DistanceTest();

	SvetoforTest();

	crossSens=CrossRoadTest();

	motorSpeed=CalculationSpeed();

	dvig.writeMicroseconds(motorSpeed);

	Outputing();

}

void DriveNoPID()
{
	dvig.writeMicroseconds(motorSpeed);
}

int DistanceTest()
{
	int dist;
	SharpIR sharp(frontDictancePin, 1080);
	dist=sharp.distance();
	if (dist > selfDistance) pedestrian = 0;
	else pedestrian = 1;
	return dist;
}

int ReadingSensorsAndLED() 
{
	int turnAngle, steering;
	linePosition = 0;
	for (int i=0; i<lineSensCount; i++)
	{
    	lineSensValues[i] = analogRead(lineSensPin[i]);
		if (lineSensValues[i] > sensitivity*lineSensSensitivity[i])
		{
			digitalWrite(lineLedPin[i], HIGH);
			steeringAngle += steeringMax*lineSensCoefs[i];
			linePosition+=steeringMax*linePropCoefs[i];
			activeSens+=1;
		}
		else
		{
			digitalWrite(lineLedPin[i], LOW);

		}
		
	}

	turnAngle = steeringAngle/activeSens;
	linePosition= linePosition/activeSens;
	return turnAngle;

}


void PinModing()
{
	for (int i=0; i<lineSensCount; i ++)
	{
		pinMode(lineLedPin[i], OUTPUT);
	    pinMode(lineSensPin[i], INPUT); 
	}
}

int WhiteTest()
{
	static int test;
	if (activeSens == 0) test =1; // Положительный результат мы на белом
	else test =0;
	return test;
}	

void SvetoforTest() 
{
  time=millis();		
  if (Serial1.available()) 
  {
  	svetSignal = Serial1.read(); 
  	oldTime = time;
  }
  else if (time-oldTime>1500)
  {
  	svetSignal = 6;
  }
  
}


int CrossRoadTest()
{
 int amountSens=0;
 for (int i=5; i<lineSensCount; i ++)
 	{
		if (lineSensValues[i] > sensitivity*lineSensSensitivity[i]) amountSens+=1;		
	}
	return amountSens;
}

int CalculationSpeed()
{
	int speed;
	if (frontDictance>selfDistance && whiteTester==0) 
		{
			if (svetSignal==2 || svetSignal==3||svetSignal==6) speed=max(maxSpeed-abs(angleTurn/brakeCoef), minSpeed);
			else if (crossSens<=4) speed=svetoforSpeed;
			else if (crossSens>4 || motorSpeed>minSpeed) speed =fullBrake;
			else speed=zeroSpeed;
		}	
	else if(motorSpeed>minSpeed) speed = fullBrake;
	else if(motorSpeed==zeroSpeed) 
	{
		speed = zeroSpeed;
		return speed;
	}
	else if((motorSpeed>reverseSpeed) && (motorSpeed<minSpeed)) speed = zeroSpeed;
	else if(motorSpeed < reverseSpeed) speed = fullBrake;
	else speed=zeroSpeed;
	return speed;
}



void Outputing()   
{
	SpeedInterpolation();
	//Stringer();

	Serial.print(map(linePosition,-300,300,-180,180));
	Serial.print(' ');
	Serial.print(map(angleTurn,-300,300,-30,30));
	Serial.print(' ');
	Serial.print(motorSpeed);
	Serial.print(' ');
	Serial.print(pedestrian);
	Serial.print(' ');
	Serial.print(svetSignal,DEC);
	Serial.print(' ');
	Serial.print(factMotorSpeed);	
	Serial.print(' ');
	for (int i=0; i<lineSensCount; i++)
	{
    	if (lineSensValues[i] > sensitivity*lineSensSensitivity[i]) Serial.print('1');
		else Serial.print('0');
    }	

	Serial.println(' ');
}

void SpeedInterpolation()
{
	factMotorSpeed = motorSpeed;
	if (1290<motorSpeed && motorSpeed< 1310) 	motorSpeed = 300;
	else if ( 1400<motorSpeed && motorSpeed<1420) motorSpeed = 270;
	else if ( 1490<motorSpeed && motorSpeed<1510) motorSpeed = 290;
	else if ( minSpeed-5<motorSpeed && motorSpeed< maxSpeed+5) motorSpeed=map(motorSpeed,minSpeed-5,maxSpeed+5,280,80);
}