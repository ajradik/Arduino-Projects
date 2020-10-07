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

class DCMotorControlledByLightLevel: public ScheduledProcess {
  
  const int dcMotorPin = 3;
  const int photoresistorPin = A0;
  
  public:
    DCMotorControlledByLightLevel() {
      pinMode(photoresistorPin, INPUT);
      pinMode(dcMotorPin, OUTPUT);
    }
  
  	void scheduledLoop() override {
      int lightValueToConstrainForLinearity = analogRead(photoresistorPin);
      analogWrite(dcMotorPin, map(constrain(lightValueToConstrainForLinearity, 264, 923), 264, 923, 0, 255));
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