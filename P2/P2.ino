//Copyright (c) 2020 Artemas J. Radik

class Process {
  public:
    virtual void run() final {
      unsigned long stamp = millis();
      
      if (stamp - latestStateUpdate >= nextStateUpdateDifferential) {
        updateState();
        latestStateUpdate = stamp;
      }
    }
      
  protected:
  	virtual void updateState() = 0;
  
  	virtual void scheduledDelay(int milliseconds) final {
      nextStateUpdateDifferential = milliseconds;
    }
  
  private:
  	unsigned int nextStateUpdateDifferential = 0;
  	unsigned long latestStateUpdate = 0;
};

class DCMotorControlledByLightLevel: public Process {
  
  const int dcMotorPin = 3;
  const int photoresistorPin = A0;
  
  public:
    DCMotorControlledByLightLevel() {
      pinMode(photoresistorPin, INPUT);
      pinMode(dcMotorPin, OUTPUT);
    }
  
  	void updateState() override {
      int lightValue = analogRead(photoresistorPin);
      analogWrite(dcMotorPin, map(lightValue, 26, 923, 0, 255));
      Serial.println(lightValue);
      scheduledDelay(50);
    }
};
 
void setup() {
  Serial.begin(9600);
}

DCMotorControlledByLightLevel dcMotorControlledByLightLevel;

void loop() {
  dcMotorControlledByLightLevel.run();
}