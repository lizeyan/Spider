#include "light.h"
#include <Wire.h>
#include <Arduino.h>
Light::Light ()
{

}

void Light::setup()
{

}

void Light::echo (int rank)
{
	if (rank < 0 || rank >= lightNum)
		return;
	Serial.print("light");
	Serial.print(rank);
	Serial.print(": ");
	Serial.print(lights[rank]); 
	Serial.println();
}

void Light::echoAll ()
{
	for (int i = 0; i < lightNum; ++i)
		echo (i);
}

void Light::update ()
{
	for (int i = 0; i < lightNum; ++i)
		lights[i] = analogRead (rank2pin[i]);
}