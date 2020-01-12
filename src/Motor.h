#ifndef MOTOR
#define MOTOR
  #include <Servo.h>

  //470 ms kell 180 fokhoz kb.
  //speed -> 0,383  fok/ms

  const float speed = 180.0 / 470.0;

  int getRequiredTimeToMoveToDegree(int currentDegree, int targetDegree) {
     short diff = currentDegree - targetDegree;
     return (abs(diff) / speed) + 1;
  }
#endif
