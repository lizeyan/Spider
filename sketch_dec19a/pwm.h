#ifndef ___PWM___
#define ___PWM___
#include <Servo.h>
#include <Adafruit_PWMServoDriver.h>
//0,1,2, attach to mega
//others, attach to pca
const int SERVOSIZE = 18;
const int SERVO_ON_MEGA = 3;
const int degreeError[SERVOSIZE] = {0, 0, 0, 0, 0, 0};
const int rankToPin[SERVOSIZE] = {8, 9, 10, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14};
#define SERVOMIN  128 // this is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX  512 // this is the 'maximum' pulse length count (out of 4096)
class Pwm
{
public:
	void set(int rank, int degree);
	void setup();
protected:
	void setForMega(int rank, int degree);
	void setForPca (int rank, int degree);
private:
	Servo servoOnMega[SERVO_ON_MEGA];
	Adafruit_PWMServoDriver pwm;
};
#endif