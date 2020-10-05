//src: https://www.tinkercad.com/things/arOOZJnayDd-arduino-blinking-led
//src: http://www.cplusplus.com/reference/cmath/sin/

#include <Servo.h>
#include <math.h>

const int redPin = 0;
const int greenPin = 1;
const int servoPin = 6;

int servoAngle = 90;
int servoAngleIncrement = 1;
Servo servo;

void setup() {
  servo.attach(servoPin);
  servo.write(servoAngle);
  pinMode(0, OUTPUT);
  pinMode(1, OUTPUT);
}

void loop(){
  
  servo.write(servoAngle);
  
  if (servoAngle == 180) {
    servoAngleIncrement -= 1;
  }
  
  else if (servoAngle == 0){
    servoAngleIncrement += 1;
  }
  
  servoAngle += servoAngleIncrement;
  
  if (servoAngle > 90) {
    digitalWrite(0, HIGH);
    digitalWrite(1, LOW);
  }
  
  else if (servoAngle < 90) {
    digitalWrite(1, HIGH);
    digitalWrite(0, LOW);
  }
  
  delay(15);
}
