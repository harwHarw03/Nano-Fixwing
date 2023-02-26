#pragma once

#include <Arduino.h>
#include <AHRS/AHRS.h>
#include <Servo.h>
#include <Remote/Remote.h>
// #include <>

Servo aileron_right;
Servo aileron_left;
Servo throttle;

class Fixwing
{
private:
/*Singleton*/
    static Fixwing *instance;
    Fixwing() {}
    Fixwing(const Fixwing &) = delete;
    Fixwing &operator=(const Fixwing &) = delete;
/*
using : AHRS *ahrs = AHRS::getInstance(); 
ex : ahrs->getAltitude
*/        
public:
/*Singleton Instance*/
    static Fixwing *getInstance()
    {   
        if (!instance)
            instance = new Fixwing();
        return instance;
    }   

    void init();
    void PID_control_update();
    void servo_output();    //next put at servos
// private: 
/*Constant*/

/*Tunning Parameters*/
    float Kp = 1.0F;
    float Ki = 0.0F;
    float Kd = 0.0F;
private :
    /*Global Variables*/
    float rollSetPoint = 0.0F;
    float rollError; 
    float rollErrorSum; 
    float rollErrorPrev; 
    float rollOutput;
    // float rollAngle = 0.0F;
public :
    int aileron_right_pwm = 1500;
    int aileron_left_pwm = 1500;
};
Fixwing *Fixwing::instance = nullptr;
Fixwing *fixwing = Fixwing::getInstance();
void Fixwing::init(){
    aileron_left.attach(5);
    aileron_right.attach(6);
}

void Fixwing::PID_control_update(){
    rollError = rollSetPoint - ahrs->get_roll();
    rollOutput = Kp * rollError + Ki * rollErrorSum + Kd * (rollError - rollErrorPrev);
    rollErrorSum += rollError;
    rollErrorPrev = rollError;

    aileron_left_pwm = aileron_left_pwm + rollOutput + inputAileron;
    aileron_right_pwm = aileron_right_pwm - rollOutput - inputAileron;
}

void Fixwing::servo_output(){
    aileron_left.writeMicroseconds(aileron_left_pwm);
    aileron_right.writeMicroseconds(aileron_right_pwm);
    throttle.writeMicroseconds(remoteThrottle);
}