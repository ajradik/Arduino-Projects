//Copyright (c) 2020 Artemas J. Radik
#include <Servo.h>

class ScheduledProcess {
  public:
    virtual void refresh() final {
      unsigned long currentTimeStamp = millis();

      if (currentTimeStamp - latestScheduledLoopIterationTimeStamp >= nextScheduledLoopIterationDifferential) {
        nextScheduledLoopIterationDifferential = 0;
        scheduledLoop();
        latestScheduledLoopIterationTimeStamp = currentTimeStamp;
      }
    }
  	
  	virtual void scheduledDelay(int milliseconds) final {
      nextScheduledLoopIterationDifferential = milliseconds;
    }
  
  	virtual void scheduledSetup() = 0;
      
  protected:
  	virtual void scheduledLoop() = 0;
  
  private:
  	unsigned int nextScheduledLoopIterationDifferential = 0;
  	unsigned long latestScheduledLoopIterationTimeStamp = 0;
};

const int dcMotorPin = 3;
const int dcMotorMin = 0;
const int dcMotorMax = 255;

const int photoresistorPin = A0;
const int photoresistorMin = 264;
const int photoresistorMax = 923;

const int ledPins [4] = {10, 11, 12, 13};

const int distanceSensorPin = 7;
const int distanceMin = 3;
const int distanceMax = 330;

const int topServoPin = 5;
const int bottomServoPin = 4;
const int servoAngleMin = 0;
const int servoAngleMax = 180;

const int tiltSensorPin = 0;
const int hobbyGearPinBottom = 8;
const int hobbyGearPinTop = 2;
const int enableHobbyGearPin = 6;
const int enableHobbyGearPinMax = 255;

class DCMotorControlledByLightLevel: public ScheduledProcess {
  
  class FlashingLEDs: public ScheduledProcess {
  
  unsigned long ledState = 0;
  
  public:
    void scheduledSetup() override {
      for (int ledPin : ledPins) {
      	pinMode(ledPin, OUTPUT);
      }
    }
  
  	void scheduledLoop() override {
      digitalWrite(ledPins[0] + ( ledState % (sizeof(ledPins)/sizeof(ledPins[0])) ), LOW);
      ledState++;
      digitalWrite(ledPins[0] + ( ledState % (sizeof(ledPins)/sizeof(ledPins[0])) ), HIGH);
    }
  };
  
  FlashingLEDs flashingLEDs;
  const int minLEDscheduledDelay = 60;
  const int maxLEDscheduledDelay = 600;
  
  public:
    void scheduledSetup() override {
      flashingLEDs.scheduledSetup();
      pinMode(photoresistorPin, INPUT);
      pinMode(dcMotorPin, OUTPUT);
    }
  
  	void scheduledLoop() override {
      flashingLEDs.refresh();
      
      int lightValue = analogRead(photoresistorPin);
      int lightValueConstrainedForLinearity = constrain(lightValue, photoresistorMin, photoresistorMax);
      analogWrite(dcMotorPin, map(lightValueConstrainedForLinearity, photoresistorMin, photoresistorMax, dcMotorMin, dcMotorMax));
      
      flashingLEDs.scheduledDelay(map(lightValueConstrainedForLinearity, photoresistorMin, photoresistorMax, maxLEDscheduledDelay, minLEDscheduledDelay));
      
      scheduledDelay(15);
    }
};

class SynchronousOppositeServosByDistance: public ScheduledProcess {
  
  Servo topServo;
  Servo bottomServo;
  
  public:
    void scheduledSetup() override {
      pinMode(distanceSensorPin, INPUT);
      pinMode(topServoPin, OUTPUT);
      pinMode(bottomServoPin, OUTPUT);
      topServo.attach(topServoPin);
      bottomServo.attach(bottomServoPin);
    }
  
  	void scheduledLoop() override {
      
      int distance = distanceInCm();
      int angle = map(constrain(distance, distanceMin, distanceMax), distanceMin, distanceMax, servoAngleMin, servoAngleMax);
      
      topServo.write(angle);
      bottomServo.write(servoAngleMax - angle);
      scheduledDelay(15);
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
};

class HobbyGearByTilt: public ScheduledProcess {
  
  public:
    void scheduledSetup() override {
      pinMode(tiltSensorPin, INPUT);
      pinMode(hobbyGearPinBottom, OUTPUT);
      pinMode(hobbyGearPinTop, OUTPUT);
      analogWrite(enableHobbyGearPin, enableHobbyGearPinMax);
    }
  
  	void scheduledLoop() override {
      
      if (digitalRead(tiltSensorPin) == 0) {
        digitalWrite(hobbyGearPinBottom, LOW);
        digitalWrite(hobbyGearPinTop, HIGH);
      }
      
      else if (digitalRead(tiltSensorPin) == 1) {
        digitalWrite(hobbyGearPinBottom, HIGH);
        digitalWrite(hobbyGearPinTop, LOW);
      }
      
      scheduledDelay(15);
    }
};

ScheduledProcess* scheduledProccesses[] = {new DCMotorControlledByLightLevel(), new SynchronousOppositeServosByDistance(), new HobbyGearByTilt()};

void setup() {
  for (ScheduledProcess* scheduledProcess : scheduledProccesses) {
    scheduledProcess->scheduledSetup();
  }
}

void loop() {
  for (ScheduledProcess* scheduledProcess : scheduledProccesses) {
    scheduledProcess->refresh();
  }
}