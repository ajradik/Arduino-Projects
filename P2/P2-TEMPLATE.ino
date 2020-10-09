/* MIT License

Copyright (c) 2020 Artemas J. Radik

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE. */

//DO NOT EDIT BELOW THIS LINE
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
//DO NOT EDIT ABOVE THIS LINE

class DCMotorControlledByLightLevel: public ScheduledProcess {

  class FlashingLEDs: public ScheduledProcess {

    //If Needed Add Your State Variables Here

    public:
      void scheduledSetup() override {
        //Type Your Code Here
      }

      void scheduledLoop() override {
        //Type Your Code Here
      }

  };

  FlashingLEDs flashingLEDs;
  //If Needed Add Your State Variables Here

  public:
    void scheduledSetup() override {
      flashingLEDs.scheduledSetup();
      //Type Your Code Here
    }

    void scheduledLoop() override {
      //Type Your Code Here
      scheduledDelay(15);
    }

};

class SynchronousOppositeServosByDistance: public ScheduledProcess {

  //If Needed Add Your State Variables Here

  public:
    void scheduledSetup() override {
      //Type Your Code Here
    }

    void scheduledLoop() override {
      //Type Your Code Here
      scheduledDelay(15);
    }

};

class HobbyGearByTilt: public ScheduledProcess {

  //If Needed Add Your State Variables Here

  public:
    void scheduledSetup() override {
      //Type Your Code Here
    }

    void scheduledLoop() override {
      //Type Your Code Here
      scheduledDelay(15);
    }

};

//DO NOT EDIT BELOW THIS LINE
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
//DO NOT EDIT ABOVE THIS LINE
