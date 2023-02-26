#include <Arduino.h>
#include "Scheduler/Scheduler.h"
#include "Scheduler/Task.h"
#include "AHRS/AHRS.h"
#include <Fixwing/Fixwing.h>
#include <Remote/Remote.h>

#include "Telemetry.h"

class AHRSUPDATE : public TimingTask
{
  public :
  AHRSUPDATE(uint32_t _rate):rate(_rate){updateTime(millis());}
  virtual void run(uint32_t now){
    ahrs->update();
    tick(rate);
  }
  private:
  uint32_t rate;
};

class FIXWING : public TimingTask
{
  public :
  FIXWING(uint32_t _rate):rate(_rate){updateTime(millis());}
  virtual void run(uint32_t now){
    remote_update();
    fixwing->PID_control_update();
    tick(rate);
  }
  private:
  uint32_t rate;
};

class Logger : public TimingTask
{
  public:
    Logger(uint32_t _rate):rate(_rate){updateTime(millis());}
    bool led_on = true;
    virtual void run(uint32_t now){
      if (led_on){digitalWrite(13, LOW); led_on = false;}
      else{digitalWrite(13, HIGH); led_on = true;}
      Serial.print("r=");
      Serial.print(ahrs->get_roll());
      Serial.print(", p=");
      Serial.print(ahrs->get_pitch());
      Serial.print(", y=");
      Serial.print(ahrs->get_yaw());
      Serial.print(", alt=");
      Serial.print(ahrs->get_baro_altitude());
      Serial.print(", Kp=");
      Serial.print(fixwing->Kp);
      Serial.print(", Ki=");
      Serial.print(fixwing->Ki);
      Serial.print(", Kd=");
      Serial.print(fixwing->Kd);
      Serial.print(", ailL=");
      Serial.print(fixwing->aileron_left_pwm);
      Serial.print(", ailR=");
      Serial.print(fixwing->aileron_right_pwm);
      Serial.println();
      tick(rate);
    } 
  private:
    uint32_t rate;
};

void setup() {
  Serial.begin(115200);
  pinMode(13, OUTPUT);
  ahrs->init();
  fixwing->init();
  remote_init();

  Logger Logger(50);
  AHRSUPDATE ahrs_update(25);
  FIXWING fixwing_task(25);
  Task *tasks[] = {&ahrs_update, &Logger, &fixwing_task};
  Scheduler scheduler(tasks, NUM_TASKS(tasks));
  while(1){scheduler.runTasks();}
}

void loop() {}