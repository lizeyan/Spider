#include "distance.h"
#include <HardwareSerial.h>
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
	serial[1] = &Serial1;
	serial[2] = &Serial2;
	serial[3] = &Serial3;
	for (int i = 1; i <= 3; ++i)
		serial[i]->begin(9600);
	emit();
}
void Distance::emit()
{
	for (int i = 1; i <= 3; ++i)
	{
		serial[i]->flush();
		serial[i]->write(0x55);
	}
}
void Distance::update ()
{
	unsigned int lenHigh = 0; // 高位
	unsigned int lenLow = 0;  // 低位
	unsigned int dist_mm = 0; // 距离

	for (int i = 1; i <= 3; ++i)
	{

		// 当串口接收缓冲区中数据大于2字节
		if(serial[i]->available() >= 2)
		{ 
			lenHigh = serial[i]->read();        // 距离的高字节
			lenLow = serial[i]->read();         // 距离的低字节
			dist_mm = lenHigh*256 + lenLow; // 计算距离值
			emit();
		}
		
		// 有效的测距的结果在1mm 到 10m 之间
		if((dist_mm > 1) && (dist_mm < 10000)) 
		{
			distances[i - 1] = dist_mm;
		}
		lenHigh = 0; // 高位
		lenLow = 0;  // 低位
		dist_mm = 0; // 距离
	}
}
void Distance::echo (int rank)
{
	if (rank < 0 || rank >= distanceNum)
		return;
	Serial.print("distance");
	Serial.print(rank);
	Serial.print(": ");
	Serial.print(distances[rank]); 
	Serial.print("mm"); 
	Serial.println();
}
void Distance::echoAll ()
{
	for (int i = 0; i < distanceNum; ++i)
		echo (i);
}