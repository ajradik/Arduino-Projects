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
  	unsigned int nextStateUpdateDifferential = 0;
  	virtual void updateState() = 0;
  
  private:
  	unsigned long latestStateUpdate = 0;
};

class DCMotorControlledByLightLevel: public Process {
  
  const int dcMotorPin = 3;
  const int photoresistorPin = A0;
  
  public:
  	
  	DCMotorControlledByLightLevel() {
      nextStateUpdateDifferential = 15;
    }
    
  	void updateState() override {
      
    }
};
 
void setup() {}

DCMotorControlledByLightLevel dcMotorControlledByLightLevel;

void loop() {
  dcMotorControlledByLightLevel.run();
}