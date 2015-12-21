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
#include "control.h"
#include <string.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

// called this way, it uses the default address 0x40
Pwm pwm;
Distance distance;
Control control;
// you can also call it with a different address you want
//Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x41);

// Depending on your servo make, the pulse width min and max may vary, you 
// want these to be as small/large as possible without hitting the hard stop
// for max range. You'll have to tweak them as necessary to match the servos you
// have!

// our servo # counter
uint8_t servonum = 0;

void setup() {
#ifdef ESP8266
  Wire.pins(2, 14);   // ESP8266 can use any two pins, such as SDA to #2 and SCL to #14
#endif
  control.setup();
  distance.setup();
  pwm.setup();
}
void runCmd()
{
  int cmd = control.getCmd();
  if (cmd == MV)
    control.moveForward (control.getArg());
  else if (cmd == TL)
    control.turn (control.getArg());
  else if (cmd == TR)
    control.turn (-control.getArg());
  else if (cmd == SC)
  {
    int rank = control.getArg();
    int degree = control.getArg();
    Serial.print("rank:");
    Serial.print(rank);
    Serial.print("degree:");
    Serial.println(degree);
    pwm.set (rank, degree);
  }
}
int loopNum = 0;
// you can use this function if you'd like to set the pulse length in seconds
// e.g. setServoPulse(0, 0.001) is a ~1 millisecond pulse width. its not precise!
void loop() 
{
  Serial.println(loopNum++);
  delay (500);
  if (control.mode() == CONTROLED)
    runCmd();
}
