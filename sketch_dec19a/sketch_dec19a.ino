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
#include <Adafruit_PWMServoDriver.h>//这个库需要从网上搜索下载。然后放到arduino安装目录的lib中

// called this way, it uses the default address 0x40
Pwm pwm;
Light light;
Distance distance;
Control control;
unsigned int next = 233;//用来生成随机数
int level;//关卡数。主要目的是在第四关有障碍物的时候加入随机性，防止陷入死循环的境地


void setup() 
{
#ifdef ESP8266
	Wire.pins(2, 14);   // ESP8266 can use any two pins, such as SDA to #2 and SCL to #14
#endif
	Serial.begin(115200);//这个端口用于蓝牙通信
	control.setup();
	distance.setup();
	light.setup();
	pwm.setup();
	control.standUp ();//让蜘蛛站起来
}

int rand()//生成随机数
{
	next = (next * 999991 + 98123);
	return next;
}
int brightest()//找到光强最大的一个光敏传感器的编号
{
	if (light[0] < light[1] && light[0] < light[2])
		return 0;
	else if (light[1] < light[0] && light[1] < light[2])
		return 1;
	else
		return 2;
}
void run()//自动模式下运行此函数
{
	
	int min = brightest ();
	if (min == 0)
	{
		if(level != 4)//如果不是第四关
			control.turnTo(4);//则直接向着光敏传感器的方向前进
		else//否则
		{
			int tmp = rand() % 6;//生成一个随机数
			if(tmp == 0) //随机朝着光敏传感器左边
				control.turnTo (0);
			else if (tmp == 1)//右边
				control.turnTo (4);
			else//正面前进。正面的概率会大一些
				control.turnTo (5);
		}
	}	
	else if (min == 1)
	{
		if(level != 4)
			control.turnTo(0);
		else
		{
			int tmp = rand() % 6;
			if(tmp == 0) 
				control.turnTo (5);
			else if (tmp == 1)
				control.turnTo (0);
			else
				control.turnTo (1);
		}
	}
	else
	{
		if(level != 4)
			control.turnTo(2);
		else
		{
			int tmp = rand() % 6;
			if(tmp == 0) 
				control.turnTo (2);
			else if (tmp == 1)
				control.turnTo (4);
			else
				control.turnTo (3);
		}
	}
	//如果light小于100，表明真正接近了目标，直接走向目标。
	//主要是为了避免将目标看作障碍导致一直碰不到目标的情况
	if(light[0] < 100 || light[1] < 100 || light[2] < 100)
	{
		control.moveForward (5);
		return;
	}
	//避障
	//距离小于150mm则倒退5步，继续行进
	int direction = control.getCenterLeg();
	if(distance[0] < 150)
		control.turnTo(3);
	if(distance[1] < 150)
		control.turnTo(1);
	if(distance[2] < 150)
		control.turnTo(5);
	//报告情况，方便调试
	Serial.print("center leg:");
	Serial.println (control.getCenterLeg());
	Serial.print ("next:");
	Serial.println(next);
	control.moveForward (2);
}
void runCmd()//受控模式
{
	int cmd = control.getCmd();//得到一个命令
	if (cmd == MV)//move
		control.moveForward (control.getArg());
	else if (cmd == TL)//turn left
		control.turn (control.getArg());
	else if (cmd == TR)// turn right
		control.turn (-control.getArg());
	else if (cmd == SC)//设定舵机角度
	{
		int rank = control.getArg();//舵机编号
		int degree = control.getArg();//角度
		pwm.set (rank, degree);
	}
	else if (cmd == SL)//设定关卡
	{
		level = control.getArg();
	}
	else if (cmd == CHANGE_AUTO)//转换到自动模式
		control.setMode (AUTO);
	else if (cmd == CHANGE_CONTROLED)//转换到受控模式
		control.setMode (CONTROLED);
}
bool checkCmd ()//自动模式下每回合检查一次是否转换到受控模式，其他命令直接忽略
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
		//自动模式下方更新并显示传感器的数值
		distance.update();
		distance.echoAll();
		light.update();
		light.echoAll();
		run();
	}
	//testServo ();
	//checkServo (4);
}

//下面是测试舵机的时候用的代码
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