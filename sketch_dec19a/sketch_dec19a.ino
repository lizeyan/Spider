#include <Wire.h>
#include "Adafruit_PWMServoDriver.h"

// called this way, it uses the default address 0x40
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();
// you can also call it with a different address you want
//Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x41);

// Depending on your servo make, the pulse width min and max may vary, you 
// want these to be as small/large as possible without hitting the hard stop
// for max range. You'll have to tweak them as necessary to match the servos you
// have!
#define SERVOMIN  150 // this is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX  600 // this is the 'maximum' pulse length count (out of 4096)

// our servo # counter
//uint8_t servonum = 0;

void setup() {
  Serial.begin(9600);
  for(int i=2; i<=6; i+=2) {
    pinMode(i, OUTPUT);
    pinMode(i+1, INPUT);
  }
  Serial.println("16 channel Servo test!");

  pwm.begin();
  
  pwm.setPWMFreq(60);  // Analog servos run at ~60 Hz updates
}

// you can use this function if you'd like to set the pulse length in seconds
// e.g. setServoPulse(0, 0.001) is a ~1 millisecond pulse width. its not precise!
void setServoPulse(uint8_t n, double pulse) {
  double pulselength;
  
  pulselength = 1000000;   // 1,000,000 us per second
  pulselength /= 60;   // 60 Hz
  Serial.print(pulselength); Serial.println(" us per period"); 
  pulselength /= 4096;  // 12 bits of resolution
  Serial.print(pulselength); Serial.println(" us per bit"); 
  pulse *= 1000;
  pulse /= pulselength;
  Serial.println(pulse);
  pwm.setPWM(n, 0, pulse);
}

void loop() {
  // Drive each servo one at a time
  //Serial.println(servonum);
  /*for (uint16_t pulselen = SERVOMIN; pulselen < SERVOMAX; pulselen++) {
    pwm.setPWM(0, 0, pulselen);
    pwm.setPWM(1, 0, pulselen);
    pwm.setPWM(2, 0, pulselen);
    pwm.setPWM(3, 0, pulselen);
    pwm.setPWM(4, 0, pulselen);
    pwm.setPWM(5, 0, pulselen);
    pwm.setPWM(6, 0, pulselen);
    pwm.setPWM(7, 0, pulselen);
    pwm.setPWM(8, 0, pulselen);
    pwm.setPWM(9, 0, pulselen);
    pwm.setPWM(10, 0, pulselen);
    pwm.setPWM(11, 0, pulselen);
    pwm.setPWM(12, 0, pulselen);
    pwm.setPWM(13, 0, pulselen);
    pwm.setPWM(14, 0, pulselen);
  }
  delay(500);
  for (uint16_t pulselen = SERVOMAX; pulselen > SERVOMIN; pulselen--) {
    pwm.setPWM(0, 0, pulselen);
    pwm.setPWM(0, 0, pulselen);
    pwm.setPWM(1, 0, pulselen);
    pwm.setPWM(2, 0, pulselen);
    pwm.setPWM(3, 0, pulselen);
    pwm.setPWM(4, 0, pulselen);
    pwm.setPWM(5, 0, pulselen);
    pwm.setPWM(6, 0, pulselen);
    pwm.setPWM(7, 0, pulselen);
    pwm.setPWM(8, 0, pulselen);
    pwm.setPWM(9, 0, pulselen);
    pwm.setPWM(10, 0, pulselen);
    pwm.setPWM(11, 0, pulselen);
    pwm.setPWM(12, 0, pulselen);
    pwm.setPWM(13, 0, pulselen);
    pwm.setPWM(14, 0, pulselen);
  }
  delay(500);*/
  for(int i=1; i<=3; i++) {
    float cm = 0.0;
    digitalWrite(i * 2, LOW); //低高低电平发一个短时间脉冲去TrigPin 
    delayMicroseconds(2); 
    digitalWrite(i * 2, HIGH); 
    delayMicroseconds(10); 
    digitalWrite(i * 2, LOW); 

    cm = pulseIn(i * 2 + 1, HIGH) / 58.0; //将回波时间换算成cm 
    cm = (int(cm * 100.0)) / 100.0; //保留两位小数 
    Serial.print("distance");
    Serial.print(i);
    Serial.print(": ");
    Serial.print(cm); 
    Serial.print("cm"); 
    Serial.println(); 
    delay(100); 
  }
  // read the input on analog pin 0:
  int sensorValue = analogRead(A0);
  Serial.print("light level1: ");
  Serial.println(sensorValue);
  delay(50);        // delay in between reads for stability
  sensorValue = analogRead(A1);
  Serial.print("light level2: ");
  Serial.println(sensorValue);
  delay(50);
  sensorValue = analogRead(A2);
  Serial.print("light level3: ");
  Serial.println(sensorValue);
  delay(50);
}
