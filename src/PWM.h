#ifndef PWM
#define PWM

  #define PWMRANGE 100

  int getPWMfromPercentage(int percentage) {
    return PWMRANGE * percentage / 100;
  }
#endif