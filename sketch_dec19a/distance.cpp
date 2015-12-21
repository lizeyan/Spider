#include "distance.h"
#include <Wire.h>

#if ARDUINO >= 100
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif
Distance::Distance ()
{
	for (int i = 0; i < distanceNum; ++i)
		distances[i] = 0;
}
void Distance::setup ()
{
	for (int i = 0; i < distanceNum; ++i)
	{
		pinMode (distanceIn[i], INPUT);
		pinMode (distanceOut[i], OUTPUT);
	}
}
void Distance::update ()
{
	for (int i = 0; i < distanceNum; ++i)
	{
    	float cm = 0.0;
	    digitalWrite(distanceOut[i], LOW); //低高低电平发一个短时间脉冲去TrigPin 
	    delayMicroseconds(2); 
	    digitalWrite(distanceOut[i], HIGH); 
	    delayMicroseconds(10); 
	    digitalWrite(distanceOut[i], LOW); 

	    cm = pulseIn(distanceIn[i], HIGH) / 58.0; //将回波时间换算成cm 
	    cm = (int(cm * 100.0)) / 100.0; //保留两位小数 

	    distances[i] = cm;
	    delay(1); 
	}
}
void Distance::echo (int rank)
{
	Serial.print("distance");
    Serial.print(rank);
    Serial.print(": ");
    Serial.print(distances[rank]); 
    Serial.print("cm"); 
    Serial.println();
}
void Distance::echoAll ()
{
	for (int i = 0; i < distanceNum; ++i)
		echo (i);
}