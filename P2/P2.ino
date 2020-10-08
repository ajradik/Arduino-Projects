//Copyright (c) 2020 Artemas J. Radik

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

class DCMotorControlledByLightLevel: public ScheduledProcess {
  
  class FlashingLEDs: public ScheduledProcess {
  
  unsigned long ledState = 0;
  
  public:
    FlashingLEDs() {
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
  const int minLEDscheduledDelay = 160;
  const int maxLEDscheduledDelay = 500;
  
  public:
    DCMotorControlledByLightLevel() {
      pinMode(photoresistorPin, INPUT);
      pinMode(dcMotorPin, OUTPUT);
    }
  
  	void scheduledLoop() override {
      flashingLEDs.refresh();
      
      int lightValue = analogRead(photoresistorPin);
      int lightValueConstrainedForLinearity = constrain(lightValue, photoresistorMin, photoresistorMax);
      analogWrite(dcMotorPin, map(lightValueConstrainedForLinearity, photoresistorMin, photoresistorMax, dcMotorMin, dcMotorMax));
      
      flashingLEDs.scheduledDelay(map(lightValueConstrainedForLinearity, photoresistorMin, photoresistorMax, minLEDscheduledDelay, maxLEDscheduledDelay));
      
      scheduledDelay(15);
    }
};


 
void setup() {
  Serial.begin(9600);
}

DCMotorControlledByLightLevel dcMotorControlledByLightLevel;

void loop() {
  dcMotorControlledByLightLevel.refresh();
}