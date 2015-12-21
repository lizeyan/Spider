#include "control.h"
#include "pwm.h"
#include "distance.h"
using namespace std;
extern Pwm pwm;
extern Distance distance;
const int bufferSize = 1024;
static char buffer[bufferSize];
static int head = 0, tail = 1;
void serialEvent ()
{
	while (Serial.available()) 
	{
		// get the new byte:
		char inChar = (char)Serial.read();
		// add it to the inputString:
		Serial.write(inChar);
		buffer[tail] = inChar;
		tail = (++tail) % bufferSize;
	}
}

void Control::setup ()
{
	Serial.begin (9600);
	Serial.write ("welcome\nmv step\ntl step\ntr step\nsc rank degree\n");
}

int Control::getCmd ()
{
	String cmd;
	while (head != tail && (buffer[head] < 'a' || buffer[head] > 'z'))
		head = (++head) % bufferSize;
	while (head != tail && buffer[head] >= 'a' && buffer[head] <= 'z')
	{
		cmd += buffer[head];
		head = (++head) % bufferSize;
	}
	/*
	Serial.print("getCmd:");
	Serial.println (cmd);
	*/
	if (cmd == String ("mv"))
		return MV;
	else if (cmd == String ("tl"))
		return TL;
	else if (cmd == String ("tr"))
		return TR;
	else if (cmd == String ("sc"))
		return SC;
}

int Control::getArg ()
{
	int arg = 0;
	while (head != tail && (buffer[head] < '0' || buffer[head] > '9'))
		head = (++head) % bufferSize;
	while (head != tail && buffer[head] >= '0' && buffer[head] <= '9')
	{
		arg *= 10;
		arg += (buffer[head] - '0');
		head = (++head) % bufferSize;
	}
	/*
	Serial.print("getArg:");
	Serial.println (arg);
	*/
	return arg;
}

inline int Control::legVirtual (int num)
{
	return (_centerLeg + num) % LEGNUM;
}

inline int Control::servoVirtual (int num, int pos)
{
	return legVirtual (num) * 3 + pos;
}

void Control::turn (int num)
{
	_centerLeg += num;
	_centerLeg %= LEGNUM;
}
void Control::standUp ()
{
	for (int i = 0; i < SERVONUM; ++i)
		pwm.set (i, 90);
}
void Control::moveForward (int steps)
{
	while (--steps >= 0)
	{
		//jugde and lift the left triangles
		for (int i = 0; i < 6; i += 2)
		{
		  pwm.set(servoVirtual(i, MID), 45);
		  pwm.set(servoVirtual(i, OUT), 135);
		}
		delay(100);
		//then move them
		pwm.set(servoVirtual(0, IN), 45);
		pwm.set(servoVirtual(2, IN), 45);
		pwm.set(servoVirtual(4, IN), 135);
		//then put down them
		delay(100);
		for (int i = 0; i < 6; i += 2)
		{
		  	pwm.set(servoVirtual(i, MID), 90);
		  	pwm.set(servoVirtual(i, OUT), 90);
		}
		delay(100);
		//lift the other three
		for (int i = 1; i < 6; i += 2)
		{
		  	pwm.set(servoVirtual(i, MID), 45);
		  	pwm.set(servoVirtual(i, OUT), 135);
		}
		delay(100);
		//reset the first triangles
		for (int i = 0; i < 6; i += 2)
		  	pwm.set(servoVirtual(i, IN), 90);
		//move the second triangles
		delay(100);
		pwm.set(servoVirtual(1, IN), 45);
		pwm.set(servoVirtual(3, IN), 135);
		pwm.set(servoVirtual(5, IN), 135);
		//put down them
		delay(100);
		for (int i = 1; i < 6; i += 2)
		{
			pwm.set(servoVirtual(i, MID), 90);
			pwm.set(servoVirtual(i, OUT), 90);
		}
		delay(100);
		//lift the first one
		for (int i = 0; i < 6; i += 2)
		{
		  	pwm.set(servoVirtual(i, MID), 45);
		  	pwm.set(servoVirtual(i, OUT), 135);
		}
		delay(100);
		//reset the second one
		for (int i = 1; i < 6; i += 2)
		  	pwm.set(servoVirtual(i, IN), 90);
		delay(100);
	}
	for (int i = 0; i < 6; i += 2)
	{
	  	pwm.set(servoVirtual(i, MID), 90);
	  	pwm.set(servoVirtual(i, OUT), 90);
	}
	delay(100);
}
