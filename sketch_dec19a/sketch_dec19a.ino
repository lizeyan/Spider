/*************************************************** 
	This is an example for our Adafruit 16-channel PWM & Servo driver
	Servo test - this will drive 16 servos, one after the other

	Pick one up today in the adafruit shop!
	------> http://www.adafruit.com/products/815

	These displays use I2C to communicate, 2 pins are required to  
	interface. For Arduino UNOs, thats SCL -> Analog 5, SDA -> Analog 4

	Adafruit invests time and resources providing this open source code, 
	please support Adafruit and open-source hardware by purchasing 
	products from Adafruit!

	Written by Limor Fried/Ladyada for Adafruit Industries.  
	BSD license, all text above must be included in any redistribution
 ****************************************************/
#include "distance.h"
#include "pwm.h"
#include "light.h"
#include "control.h"
#include <string.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

// called this way, it uses the default address 0x40
Pwm pwm;
Light light;
Distance distance;
Control control;
unsigned int next = 233;
int level;
// you can also call it with a different address you want
//Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x41);

// Depending on your servo make, the pulse width min and max may vary, you 
// want these to be as small/large as possible without hitting the hard stop
// for max range. You'll have to tweak them as necessary to match the servos you
// have!

// our servo # counter
uint8_t servonum = 0;

void setup() {
#ifdef ESP8266
	Wire.pins(2, 14);   // ESP8266 can use any two pins, such as SDA to #2 and SCL to #14
#endif
	Serial.begin(115200);
	control.setup();
	distance.setup();
	light.setup();
	pwm.setup();
	control.standUp ();
}

int rand()
{
	next = (next * 999991 + 98123);
	return next;
}

void run()
{
	int min = -1;
	if (light[0] < light[1] && light[0] < light[2])
		min = 0;
	else if (light[1] < light[0] && light[1] < light[2])
		min = 1;
	else
		min = 2;				
	if (min == 0)
	{
		if(level != 4)
		{
			control.turnTo(4);
		}
		else
		{
			int tmp = rand() % 6;
			if(tmp == 0) 
			{
				control.turnTo (0);
			}
			else if (tmp == 1)
			{
				control.turnTo (4);
			}
			else
				control.turnTo (5);
		}
	}	
	else if (min == 1)
	{
		if(level != 4)
		{
			control.turnTo(0);
		}else
		{
			int tmp = rand() % 6;
			if(tmp == 0) 
			{
				control.turnTo (5);
			}
			else if (tmp == 1)
			{
				control.turnTo (0);
			}
			else
				control.turnTo (1);
		}
	}
	else
	{
		if(level != 4)
		{
			control.turnTo(2);
		}else
		{
			int tmp = rand() % 6;
			if(tmp == 0) 
			{
				control.turnTo (2);
			}
			else if (tmp == 1)
			{
				control.turnTo (4);
			}
			else
				control.turnTo (3);
		}
	}
	if(light[0] < 100 || light[1] < 100 || light[2] < 100)
	{
		control.moveForward (5);
		return;
	}
	int direction = control.getCenterLeg();
	if(distance[0] < 150)
	{
		control.turnTo(3);
	}
	if(distance[1] < 150)
	{
		control.turnTo(1);
	}
	if(distance[2] < 150)
	{
		control.turnTo(5);
	}
	Serial.print("center leg:");
	Serial.println (control.getCenterLeg());
	Serial.print ("next:");
	Serial.println(next);
	control.moveForward (2);
}
void runCmd()
{
	int cmd = control.getCmd();
	if (cmd == MV)
		control.moveForward (control.getArg());
	else if (cmd == TL)
		control.turn (control.getArg());
	else if (cmd == TR)
		control.turn (-control.getArg());
	else if (cmd == SC)
	{
		int rank = control.getArg();
		int degree = control.getArg();
		pwm.set (rank, degree);
	}
	else if (cmd == SL)
	{
		level = control.getArg();
	}
	else if (cmd == CHANGE_AUTO)
		control.setMode (AUTO);
	else if (cmd == CHANGE_CONTROLED)
		control.setMode (CONTROLED);
}
bool checkCmd ()
{
	int cmd = control.getCmd();
	if (cmd == CHANGE_CONTROLED)
	{
		control.setMode (CONTROLED);
		return false;
	}
	return true;
}
int loopNum = 0;
// you can use this function if you'd like to set the pulse length in seconds
// e.g. setServoPulse(0, 0.001) is a ~1 millisecond pulse width. its not precise!
void loop() 
{
	Serial.println(loopNum++);
	delay(500);
	if (control.mode() == CONTROLED)
		runCmd();
	else if (control.mode() == AUTO && checkCmd())
	{
		distance.update();
		distance.echoAll();
		light.update();
		light.echoAll();
		run();
	}
	//testServo ();
	//checkServo (4);
}

void testServo()
{
	for (int i = 60; i <= 120; i += 1)
	{
		for (int j = 0; j < 18; ++j)
			pwm.set (j, i);
		delay (10);
	}
	delay (500);
	for (int i = 120; i >= 60; i -= 1)
	{
		for (int j = 0; j < 18; ++j)
			pwm.set (j, i);
		delay (10);
	}

	delay (500);
}

void checkServo (int rank)
{
	pwm.set (rank, 0);
	delay (2000);
	pwm.set (rank, 45);
	delay (2000);
	pwm.set (rank, 90);
	delay (2000);
	pwm.set (rank, 135);
	delay (2000);
	pwm.set (rank, 180);
	delay (2000);
}