#include "pwm.h"
#include <Servo.h>
void Pwm::setup ()
{
	for (int i = 0; i < SERVO_ON_MEGA; ++i)
		servoOnMega[i].attach (rankToPin[i]);

	pwm.begin();
  
  	pwm.setPWMFreq(60);  // Analog servos run at ~60 Hz updates

  	yield();
}

void Pwm::setForMega (int rank, int degree)
{
	servoOnMega[rank].write(degree);
	delay(2);
}

void Pwm::setForPca (int rank, int degree)
{
	int val = map (degree, 0, 180, SERVOMIN, SERVOMAX);
	pwm.setPWM (rankToPin[rank], 0, val);
}

void Pwm::set (int rank, int degree)
{
	Serial.print("rank:");
	Serial.print(rank);
	Serial.print("degree:");
	Serial.println(degree);
	int realDegree = degree + degreeError[rank];
	if (rank < SERVO_ON_MEGA)
		setForMega (rank, realDegree);
	else
		setForPca (rank, realDegree);
}