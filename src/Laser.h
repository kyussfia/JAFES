#include "HardwareSerial.h"
#ifndef LASER
#define LASER
  #include "PWM.h"
  #include "Motor.h"

  class Laser {
   public:
      Laser(int pin, short percentage) {
        this->pin = pin;
        this->pwm = percentage;
     }

     void on() {
        analogWrite(this->pin, getPWMfromPercentage(this->pwm));   
     }

     void off() {
        analogWrite(this->pin, 0);
     }

     void initServos(int servoPin1, int servoPin2) {
         Serial.println("Init servos:");
         uint8_t s = this->Y.attach(servoPin1);
         Serial.printf("\t\tY:= %d\n", s);
         s = this->X.attach(servoPin2);
         Serial.printf("\t\tX:= %d\n", s);
         this->X.write(0);
         this->Y.write(180);
         delay(300);
         this->X.write(60);
         this->Y.write(120);
         delay(300);
         this->X.write(90);
         this->Y.write(90);
         delay(300);
         this->X.write(120);
         this->Y.write(60);
         delay(300);
         this->X.write(180);
         this->Y.write(0);
         delay(300);
         this->X.write(0);
         this->Y.write(180);
         delay(1000); //500 would be enough, but this is good now
     }

     void moveSeparate(short degreeX, short degreeY) {
        int XtimeToMove = getRequiredTimeToMoveToDegree(this->X.read(), degreeX);     
        int YtimeToMove = getRequiredTimeToMoveToDegree(this->Y.read(), degreeY);
        this->X.write(degreeX);
        this->Y.write(degreeY);
        delay(max(XtimeToMove, YtimeToMove)); //wait to reach position
     }

     void moveTogether(short degree) {
        int XtimeToMove = getRequiredTimeToMoveToDegree(this->X.read(), degree);     
        int YtimeToMove = getRequiredTimeToMoveToDegree(this->Y.read(), 180 - degree);
        this->X.write(degree);
        this->Y.write(180 - degree);
        delay(max(XtimeToMove, YtimeToMove)); //wait to reach position
     }

   private:
    Servo X;
    Servo Y;
    short pin;
    short pwm;
  };
#endif
