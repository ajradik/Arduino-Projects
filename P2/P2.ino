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
      
  protected:
  	virtual void scheduledLoop() = 0;
  
  	virtual void scheduledDelay(int milliseconds) final {
      nextScheduledLoopIterationDifferential = milliseconds;
    }
  
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
  
  unsigned long ledState = 0;
  
  public:
    DCMotorControlledByLightLevel() {
      pinMode(photoresistorPin, INPUT);
      pinMode(dcMotorPin, OUTPUT);
      
      for (int ledPin : ledPins) {
      	pinMode(ledPin, OUTPUT);
      }
    }
  
  	void scheduledLoop() override {
      int lightValue = analogRead(photoresistorPin);
      int lightValueConstrainedForLinearity = constrain(lightValue, photoresistorMin, photoresistorMax);
      analogWrite(dcMotorPin, map(lightValueConstrainedForLinearity, photoresistorMin, photoresistorMax, dcMotorMin, dcMotorMax));
      
      digitalWrite(ledPins[0] + (ledState % 4), LOW);
      ledState++;
      digitalWrite(ledPins[0] + (ledState % 4), HIGH);
      
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