#include "control.h"
#include "pwm.h"
#include "distance.h"
#include "light.h"
using namespace std;
extern Pwm pwm;
extern Distance distance;
extern Light light;
const int bufferSize = 1024;
static char buffer[bufferSize];//一个循环队列，做串口通信的缓冲区
static int head = 0, tail = 1;
void serialEvent ()//串口收到信息的时候
{
	while (Serial.available()) 
	{
		// get the new byte:
		char inChar = (char)Serial.read();//接受一个字符
		// add it to the inputString:
		Serial.write(inChar);//把这个字符通过串口发送回去
		buffer[tail] = inChar;//把这个字符加入缓冲区
		tail = (++tail) % bufferSize;
	}
	Serial.println();//换行
}

void Control::setup ()
{
	Serial.begin (9600);
	Serial.write ("welcome\nmv step\ntl step\ntr step\nsc rank degree\n");
}

int Control::getCmd ()//从缓冲区中得到一个英文单词，并比对之
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

int Control::getArg ()//从缓冲区中得到一个整数
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

//我们的蜘蛛是中心对称的，所以在写代码的时候只针对其中一个方向写了前进。
//其他方向简单地可以通过这个函数的转换来实现
inline int Control::legVirtual (int num)//得到当前num对应的实际的腿的编号
{
	return (_centerLeg + num) % LEGNUM;
}

inline int Control::servoVirtual (int num, int pos)//得到num对应的某个位置的舵机的编号
{
	return legVirtual (num) * 3 + pos;
}

void Control::turn (int num)//变换方向
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
	int delaytime = 80;//每个动作之间间隔的时间
	//间隔时间太短会导致更多舵机同时处于高负载状态，导致电源过载。
	int outup = 145;//外侧舵机抬起时的角度
	int outdown = 145;//外侧舵机站立时的角度
	int middown = 145;//中间舵机站立时的角度
	int midup = 180;//中间舵机抬起时的角度
	//转换形态，把一对前腿向后转，转变成左右镜面对称的形式。Figure2
	pwm.set (servoVirtual(0, IN), 120);
	pwm.set (servoVirtual(5, IN), 60);
	int tmpSteps = steps;
	while (--tmpSteps >= 0)
	{
		if((steps - tmpSteps) % 5 == 0)//每走五步停一次。
			//主要是由于三条腿的力量不足，隔一段时间需要六条腿共同作用支撑一下机身
		{
			standUp();
			delay(1000);
		}
		//下面是运动的代码。
		//主要思路是把六条腿分成两个三角形，交替前移
		//抬起第一个三角形(0,2,4)
		for (int i = 0; i < 6; i += 2)
		{
		  pwm.set(servoVirtual(i, MID), midup);
		  pwm.set(servoVirtual(i, OUT), outup);
		}
		//把这个三角形往前挪
		pwm.set(servoVirtual(0, IN), 90);
		pwm.set(servoVirtual(2, IN), 60);
		pwm.set(servoVirtual(4, IN), 120);
		//放下这个三角形.Figure3
		delay(delaytime);
		for (int i = 0; i < 6; i += 2)
		{
		  	pwm.set(servoVirtual(i, MID), middown);
		  	pwm.set(servoVirtual(i, OUT), outdown);
		}
		delay(delaytime);
		//抬起另外一个三角形（1，3，5）
		for (int i = 1; i < 6; i += 2)
		{
		  	pwm.set(servoVirtual(i, MID), midup);
		  	pwm.set(servoVirtual(i, OUT), outup);
		}
		delay(delaytime >> 1);
		//第一个三角形回撤到初始位置，因为此时它们着地了，机器人就会前进
		pwm.set (servoVirtual(0, IN), 120);
		pwm.set (servoVirtual(2, IN), 90);
		pwm.set (servoVirtual(4, IN), 90);
		//同时第二个三角形前移.Figure4
		pwm.set(servoVirtual(1, IN), 75);
		pwm.set(servoVirtual(3, IN), 120);
		pwm.set(servoVirtual(5, IN), 90);
		//把第二个三角形放下
		delay(delaytime);
		for (int i = 1; i < 6; i += 2)
		{
			pwm.set(servoVirtual(i, MID), middown);
			pwm.set(servoVirtual(i, OUT), outdown);
		}
		delay(delaytime);
		//第二个三角形回撤到初始位置
		pwm.set (servoVirtual(1, IN), 90);
		pwm.set (servoVirtual(3, IN), 90);
		pwm.set (servoVirtual(5, IN), 60);
	}

	//六条腿均放下
	for (int i = 0; i < 6; i += 2)
	{
		pwm.set (servoVirtual(i, MID), middown);
		pwm.set (servoVirtual(i, OUT), outdown);
	}
	delay (delaytime);
	//六条腿都恢复到正常的中心对称的位置
	pwm.set (servoVirtual(0, IN), 90);
	pwm.set (servoVirtual(5, IN), 90);
}
