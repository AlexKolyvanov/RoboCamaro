#line 1 "E:/ScetchbookForArduibo/RC_manipulatorV2/RC_manipulatorV2.ino"
//RC_manipulatorV2.ino

#include <Servo.h>  					//подключаем библиотеку серво

#include <Arduino.h>
void RadioControl();
void ScanRC();
#line 5
const int CH1 = 8;						// Назначаем мины для каналов управления с R/C передатчика
const int CH2 = 9;
//const int CH3 = 10;
const int CH4 = 11;
const int CH5 =12;
const int CH6 =13;

const long maxtime = 20000;         // Вермя таймаута для сканирования сигнала (неиспользуется)
const long maxSpeed = 1;			// изменение угла серво за одну итерацию
	
int current1 = 90; 					// Значения которые мы подадим на сервы 
int current2 = 90;					// также являются начальными значениями
int current3 = 90;
int current4 = 90;
int current5 = 90;
int current6 = 90;

int SpeedStiring = 0;				// переменая для скорости поворота

const int SerialMonSpeed=9600;       // скорость последовательного порта для монитора
long Dur1=0;						// переменные для записывания значения ШИМ с R/C приемника
long Dur2=0;
long Dur3=0;
long Dur4=0;
long Dur5=0;
long Dur6=0;

Servo servO;						// переменная серво в основании
Servo servL;						// серво локоть
Servo servK;						// серво кисть
Servo servZ;						// серво захват

const int IA1 = 2;  				// Управляющий вывод 1   ( на драйвер двигателя поворотов)
const int IA2 = 3;  				// Управляющий вывод 2


unsigned long timeloop;					// время 


void setup() {
  pinMode(CH1, INPUT);				// Назначаем пины на чтение
  pinMode(CH2, INPUT);
  //pinMode(CH3, INPUT);
  pinMode(CH4, INPUT);
  pinMode(CH5, INPUT);
  pinMode(CH6, INPUT);
	
  servZ.attach(4);						// прикреплем сервы к пинам
  servK.attach(5);
  servL.attach(6);
  servO.attach(7);
  Serial.begin(SerialMonSpeed);			//открываем последовательный порт

  digitalWrite(CH4, HIGH); // подтягивающий резистор
}


void loop() {
	timeloop = millis();
	if (timeloop % 10 == 0)	RadioControl();					
	
}


void RadioControl()
{
	
	ScanRC();

	if (current1 >0 && current1 < 180) current1 +=Dur1;
	else if( current1 <=0) current1 =1 ;
	else current1 =179;

	if (current2 >0 && current2 < 180) current2 +=Dur2;
	else if( current2 <=0) current2 =1 ;
	else current2 =179;

	if (current6 >0 && current6 < 180) current6 +=Dur6;
	else if( current6 <=0) current6 =1 ;
	else current6 =179;

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
      Serial.println(SpeedStiring, DEC);
    }

    else 
    { // иначе вращаем ротор в другую сторону
      analogWrite(IA1, LOW);
      analogWrite(IA2, -SpeedStiring);
    }

	servO.write(current5);
	servZ.write(current1);
	servK.write(current2);
	servL.write(current6);
	//Serial.println(current3, DEC);
}


void ScanRC()
{
	Dur1 = pulseIn(CH1, HIGH);     	// Засекаем время сигнала на пине СH1, соответствует сигналу с пульта
	//Serial.println(Dur1, DEC);	// Для сервы захвата 
	Dur1=map(Dur1,952,1955,-5,5); 	// интерпалирем полученое значение в диапозон - 5 ..... 5

	Dur2 = pulseIn(CH2, HIGH);     // для кисти
	Dur2=map(Dur2,885,1900,-5,5);

	//Dur3 = pulseIn(CH3, HIGH);		
	
	//Dur3=map(Dur3,952,1967,0,180);

	Dur4 = pulseIn(CH4, HIGH);   	// для поворотного двигателя
	//Serial.println(Dur4, DEC);

	//Serial.println(Dur4, DEC);             // 981 ... 1940    // Крутая система интерполяции, чтобы перепрыгнуть момент, когда у движка не хватает силы 
	if (1415 <= Dur4 && Dur4 <= 1485) {SpeedStiring = 1; }		// и устранить нулевую погрешность
	else if (1375 <= Dur4  && Dur4 <= 1525) {SpeedStiring = map(Dur4, 1050, 1850 , -125, 125);}
	else if (981 <= Dur4  && Dur4 <= 1375 ) {SpeedStiring=map(Dur4,981,1050,-180,-125);}
	else if ( 1525 <= Dur4  && Dur4 <= 1970) {SpeedStiring=map(Dur4,981,1970,125, 180);}
	else {SpeedStiring =1; }
	//Serial.println(SpeedStiring, DEC); 

	Dur5 = pulseIn(CH5, HIGH);     	// Для сервы в основании
	
	Dur5=map(Dur5,986,1972,0,180);
	

	Dur6 = pulseIn(CH6, HIGH);		// для локтя
	Dur6=map(Dur6,1000,2000,-5,5);
}
