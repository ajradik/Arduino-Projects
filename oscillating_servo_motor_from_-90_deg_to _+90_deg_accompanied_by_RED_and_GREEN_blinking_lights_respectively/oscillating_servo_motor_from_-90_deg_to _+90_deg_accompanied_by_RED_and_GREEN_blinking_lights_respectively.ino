//src: https://www.tinkercad.com/things/arOOZJnayDd-arduino-blinking-led

#include <Servo.h>

const int redPin = 0;
const int greenPin = 1;
const int servoPin = 6;

int servoAngle = 90;
int servoAngleIncrement = 1;
Servo servo;

void setup() {
  servo.attach(servoPin);
  servo.write(servoAngle);
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
}

void loop(){
  
  servo.write(servoAngle);
  
  if (servoAngle == 180) {
    servoAngleIncrement = -1;
  }
  
  else if (servoAngle == 0){
    servoAngleIncrement = 1;
  }
  
  servoAngle += servoAngleIncrement;
  
  if (servoAngle > 90) {
    digitalWrite(redPin, HIGH);
    digitalWrite(greenPin, LOW);
  }
  
  else if (servoAngle < 90) {
    digitalWrite(greenPin, HIGH);
    digitalWrite(redPin, LOW);
  }
  
  delay(10);
}
