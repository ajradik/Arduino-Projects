//src: https://www.arduino.cc/reference/en/language/functions/advanced-io/tone/
//src: https://www.programmingelectronics.com/an-easy-way-to-make-noise-with-arduino-using-tone/#:~:text=The%20frequency%20is%20an%20unsigned,our%20ears%20are%20most%20tuned.
//src: https://www.tinkercad.com/things/9AzmS567fJN-ultrasonic-distance-sensor

const int distanceSensorPin = 6;
const int piezoPin = 11;

void setup(){}

void loop() {
  long distance = distanceInCm();
  distance = constrain(distance, 3, 330);
  int frequency = map(distance, 3, 330, 2000, 5000);
  tone(piezoPin, frequency);
  delay(15);
}

long distanceInCm() {
  //code citation https://www.tinkercad.com/things/9AzmS567fJN-ultrasonic-distance-sensor
  
  // establish variables for duration of the ping,
  // and the distance result in inches and centimeters:
  long duration, cm;

  // The PING))) is triggered by a HIGH pulse of 2 or more microseconds.
  // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
  pinMode(distanceSensorPin, OUTPUT);
  digitalWrite(distanceSensorPin, LOW);
  delayMicroseconds(2);
  digitalWrite(distanceSensorPin, HIGH);
  delayMicroseconds(5);
  digitalWrite(distanceSensorPin, LOW);

  // The same pin is used to read the signal from the PING))): a HIGH
  // pulse whose duration is the time (in microseconds) from the sending
  // of the ping to the reception of its echo off of an object.
  pinMode(distanceSensorPin, INPUT);
  duration = pulseIn(distanceSensorPin, HIGH);
  
  // The speed of sound is 340 m/s or 29 microseconds per centimeter.
  // The ping travels out and back, so to find the distance of the
  // object we take half of the distance travelled.
  return duration / 29 / 2;
}
