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

void Control::moveForward (int num)
{

}
