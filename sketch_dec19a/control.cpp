#include "control.h"
#include "pwm.h"
#include "distance.h"
#include "light.h"
using namespace std;
extern Pwm pwm;
extern Distance distance;
extern Light light;
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
	Serial.println();
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
	else if (cmd == String ("control"))
		return CHANGE_CONTROLED;
	else if (cmd == String ("sl"))
		return SL;
	else if (cmd == String ("auto"))
		return CHANGE_AUTO;
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

void Control::turnTo (int rank)
{
	_centerLeg = rank;
}
void Control::standUp ()
{
	for (int i = 0; i < LEGNUM; ++i)
	{
		int in = i * 3;
		int mid = i * 3 + 1;
		int out = i * 3 + 2;
		pwm.set (in, 90);
		pwm.set (mid, 145);
		pwm.set (out, 145);
	}
}
void Control::moveForward (int steps)
{
	int delaytime = 80;
	int outup = 145;
	int outdown = 145;
	int middown = 145;
	int midup = 180;
	int offset = 10;
	pwm.set (servoVirtual(0, IN), 120);
	//pwm.set (servoVirtual(3, IN), 120);
	//pwm.set (servoVirtual(2, IN), 60);
	pwm.set (servoVirtual(5, IN), 60);
	int tmpSteps = steps;
	while (--tmpSteps >= 0)
	{
		if((steps - tmpSteps) % 5 == 0)
		{
			standUp();
			delay(1000);
		}
		//jugde and lift the left triangles
		for (int i = 0; i < 6; i += 2)
		{
		  pwm.set(servoVirtual(i, MID), midup);
		  pwm.set(servoVirtual(i, OUT), outup);
		}
		//then move them
		pwm.set(servoVirtual(0, IN), 90);
		pwm.set(servoVirtual(2, IN), 60);
		pwm.set(servoVirtual(4, IN), 120);
		//then put down them
		delay(delaytime);
		for (int i = 0; i < 6; i += 2)
		{
		  	pwm.set(servoVirtual(i, MID), middown);
		  	pwm.set(servoVirtual(i, OUT), outdown);
		}
		delay(delaytime);
		//lift the other three
		for (int i = 1; i < 6; i += 2)
		{
		  	pwm.set(servoVirtual(i, MID), midup);
		  	pwm.set(servoVirtual(i, OUT), outup);
		}
		delay(delaytime >> 1);
		//reset the first triangles
		pwm.set (servoVirtual(0, IN), 120);
		pwm.set (servoVirtual(2, IN), 90);
		pwm.set (servoVirtual(4, IN), 90);
		//move the second triangles
		pwm.set(servoVirtual(1, IN), 75);
		pwm.set(servoVirtual(3, IN), 120);
		pwm.set(servoVirtual(5, IN), 90);
		//put down them
		delay(delaytime);
		for (int i = 1; i < 6; i += 2)
		{
			pwm.set(servoVirtual(i, MID), middown);
			pwm.set(servoVirtual(i, OUT), outdown);
		}
		delay(delaytime);
		for (int i = 1; i < 6; i += 2)
		  	pwm.set(servoVirtual(i, IN), 90);
		pwm.set (servoVirtual(1, IN), 90);
		pwm.set (servoVirtual(3, IN), 90);
		pwm.set (servoVirtual(5, IN), 60);
	}
	for (int i = 0; i < 6; i += 2)
	{
		pwm.set (servoVirtual(i, MID), middown);
		pwm.set (servoVirtual(i, OUT), outdown);
	}
	delay (delaytime);
	pwm.set (servoVirtual(0, IN), 90);
	pwm.set (servoVirtual(3, IN), 90);
	pwm.set (servoVirtual(2, IN), 90);
	pwm.set (servoVirtual(5, IN), 90);
}
