// ManipulatorV3.ino

#include <Servo.h>  					//подключаем библиотеку серво

const long maxtime = 20000;         // Вермя таймаута для сканирования сигнала (неиспользуется)
const long maxSpeed = 1;			// изменение угла серво за одну итерацию

int current1 = 90; 					// Значения которые мы подадим на сервы 
int current2 = 90;					// также являются начальными значениями
int current3 = 90;
int current4 = 90;


int SpeedStiring = 0;				// переменая для скорости поворота

const int SerialMonSpeed = 9600;       // скорость последовательного порта для монитора
long Dur1=0;						// переменные для записывания значения ШИМ с R/C приемника
long Dur2=0;
long Dur3=0;
long Dur4=0;
int flag=0;


Servo servO;						// переменная серво в основании
Servo servL;						// серво локоть
Servo servK;						// серво кисть
Servo servZ;						// серво захват

const int IA1 = 2;  				// Управляющий вывод 1   ( на драйвер двигателя поворотов)
const int IA2 = 3;  				// Управляющий вывод 2


unsigned long timeloop;					// время 


void setup() {

  servZ.attach(4);						// прикреплем сервы к пинам
  servK.attach(5);
  servL.attach(6);
  servO.attach(7);
  Serial.begin(SerialMonSpeed);			//открываем последовательный порт


  servZ.write(current1);
  servK.write(current2);
  servL.write(current3);
  servO.write(current4);

}


void loop() {
	int i;
	if (flag == 0)
	{
		for(i=90; i<160; i++)
		{
			servZ.write(i);
			if (i<158) flag =0;
			else flag =1;
		}
	}
	else if (flag == 1)
	{
		for(i=160; i>90; i--)
		{
			servZ.write(i);
			if (i>92) flag =1;
			else flag =2;
		}
	}
	else if (flag == 2)
	{
		for(i=90; i<120; i++)
		{
			servK.write(i);
			if (i<118) flag =2;
			else flag =3;
		}
	}
	else if (flag == 3)
	{
		for(i=120; i>90; i--)
		{
			servZ.write(i);
			if (i>92) flag =3;
			else flag =4;
		}
	}

	else if (flag == 4)
	{
		for(i=90; i<150; i++)
		{
			servL.write(i);
			if (i<148) flag =4;
			else flag =5;
		}
	}
	else if (flag == 5)
	{
		for(i=150; i>45; i--)
		{
			servL.write(i);
			if (i>47) flag =5;
			else flag =0;
		}
	}
}
/*

void RadioControl()
{
	

	{ // если число положительное, вращаем в одну сторону 
      analogWrite(IA1, SpeedStiring); //
      analogWrite(IA2, LOW);      
      //Serial.println(SpeedStiring, DEC);
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

*/


