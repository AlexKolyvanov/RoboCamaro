#line 1 "E:/ScetchbookForArduibo/RC_manipulator/RC_manipulator.ino"
//RC_manipulator.ino
#include <Servo.h>

#include <Arduino.h>
#line 4
const int CH1 = 8;
const int CH2 = 9;
const int CH3 = 10;
const int CH4 = 11;
const int CH5 =12;
//const int CH6 =13;

const long maxtime = 20000;
const long maxSpeed = 3;

int current1 = 90;
int current2 = 90;
int current3 = 90;
int current4 = 90;
int current5 = 90;
int current6 = 90;

int SpeedStiring = 0;

const int SerialMonSpeed=9600;
long Dur1=0;
long Dur2=0;
long Dur3=0;
long Dur4=0;
long Dur5=0;
long Dur6=0;

Servo servO;
Servo servL;
Servo servK;
Servo servZ;

const int IA1 = 2;  // Управляющий вывод 1
const int IA2 = 3;  // Управляющий вывод 2

void setup() {
  // put your setup code here, to run once:
  pinMode(CH1, INPUT);
  pinMode(CH2, INPUT);
  pinMode(CH3, INPUT);
  pinMode(CH4, INPUT);
  pinMode(CH5, INPUT);
  //pinMode(CH6, INPUT);
	
  servZ.attach(4);
  servK.attach(5);
  servL.attach(6);
  servO.attach(7);
  Serial.begin(SerialMonSpeed);

  digitalWrite(CH4, HIGH); // подтягивающий резистор
}


void loop() {
	Dur1 = pulseIn(CH1, HIGH);
	//Serial.println(Dur1, DEC);
	Dur1=map(Dur1,952,1955,-5,5);

//
	Dur2 = pulseIn(CH2, HIGH);
	Dur2=map(Dur2,885,1900,-5,5);

	Dur3 = pulseIn(CH3, HIGH);
	
	Dur3=map(Dur3,952,1967,-5,5);

	Dur4 = pulseIn(CH4, HIGH);
	Serial.println(Dur4, DEC);
	SpeedStiring=map(Dur4,981,1940,-130,130);

	Dur5 = pulseIn(CH5, HIGH);
	
	Dur5=map(Dur5,986,1972,0,180);
	
	
	

	//Dur6 = pulseIn(CH6, HIGH);
	//Dur6=map(Dur6,1000,2000,-5,5);

	/*if (current1 < Dur1) current1+=maxSpeed;
	if (current1 > Dur1) current1-=maxSpeed;

	if (current1 < Dur2) current2+=maxSpeed;
	if (current1 > Dur2) current2-=maxSpeed;

	if (current1 < Dur3) current3+=maxSpeed;
	if (current1 > Dur3) current3-=maxSpeed;

	if (70 < Dur4) current3+=maxSpeed;
	if (110 > Dur4) current3-=maxSpeed;

	if (current1 < Dur5) current3+=maxSpeed;
	if (current1 > Dur5) current3-=maxSpeed;*/

	if (current1 >0 && current1 < 180) current1 +=Dur1;
	else if( current1 <=0) current1 =1 ;
	else current1 =179;

	if (current2 >0 && current2 < 180) current2 +=Dur2;
	else if( current2 <=0) current2 =1 ;
	else current2 =179;

	if (current3 >0 && current3 < 180) current3 +=Dur3;
	else if( current3 <=0) current3 =1 ;
	else current3 =179;

	//if (current4 > 0 && current4 < 181) current4 +=Dur4;
	 if (current5 < Dur5) current5+=maxSpeed;
	 if (current5 > Dur5) current5-=maxSpeed;
	//if (current6 > 0 && current6 < 181) current6+=Dur6;
	/*if (70 < Dur6)*/ 
	//1if (110 > Dur6) current6-=maxSpeed;

	//Serial.println(SpeedStiring, DEC);
	if (SpeedStiring > 0) 
	{ // если число положительное, вращаем в одну сторону 
      analogWrite(IA1, SpeedStiring); //
      analogWrite(IA2, LOW);      
    }
    else 
    { // иначе вращаем ротор в другую сторону
      analogWrite(IA1, LOW);
      analogWrite(IA2, -SpeedStiring);
    }

	servO.write(current5);
	servZ.write(current1);
	servK.write(current2);
	servL.write(current3);
	//Serial.println(current3, DEC);
	delay(25);

}

