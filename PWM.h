#ifndef PWM
#define PWM
  int getPWMfromPercentage(int percentage) {
    return PWMRANGE * percentage / 100;
  }
#endif
