#ifndef ___PWM___
#define ___PWM___
#include <Servo.h>
#include <Adafruit_PWMServoDriver.h>
//0,1,2, attach to mega
//others, attach to pca
const int SERVOSIZE = 18;
const int SERVO_ON_MEGA = 12;//板子上舵机的数目。接在板子上的舵机必须是[0, SERVO_ON_MEGA)号。之后的全部都是在扩展板上的
//舵机的角度设置，修正误差
const int degreeError[SERVOSIZE] = {0, 0, 10, -20, -15, 15, 0, 0, -5, 15, 5, -15, -10, -10, -10, -12, -20, -30};
//									0, 1,  2,  3,    4,  5, 6, 7,  8,  9,10,  11,  12,13,14,15,16,17
//每个舵机对应的接口的编号									
const int rankToPin[SERVOSIZE] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 9, 10, 11, 12, 13, 14};//每个舵机对应的实际接口的编号
#define SERVOMIN  150 // this is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX  600 // this is the 'maximum' pulse length count (out of 4096)
class Pwm
{
public:
	void set(int rank, int degree);//设定舵机角度的统一接口
	void setup();//初始化舵机
protected:
	void setForMega(int rank, int degree);//设定角度，在板子上的舵机
	void setForPca (int rank, int degree);//设定角度，在扩展板上的舵机
private:
	Servo servoOnMega[SERVO_ON_MEGA];
	Adafruit_PWMServoDriver pwm;//
};
#endif