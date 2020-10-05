//src: https://www.arduino.cc/reference/en/libraries/servo/
//src: https://www.arduino.cc/reference/en/language/functions/math/map/
//src: https://www.arduino.cc/reference/en/language/functions/math/constrain/
//src: https://www.tinkercad.com/things/aHVJVUTZJ1t-servo-motor-control-wpotentiometer
//src: https://www.arduino.cc/en/Tutorial/Knob

#include <Servo.h> 

const int potPin = 0;
const int servoPin = 6;

Servo servo;

void setup() {
  servo.attach(servoPin);
}

void loop() {
  int potAngle = analogRead(potPin);
  int potAngleRestricted = constrain(potAngle, 0, 1023);
  int servoAngle = map(potAngleRestricted, 0, 1023, 0, 180);
  servo.write(servoAngle);
  delay(15);
}
